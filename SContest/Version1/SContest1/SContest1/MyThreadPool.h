#pragma once

#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <future>
#include <atomic>
#include <vector>
#include <queue>

class TaskExecutor
{
	using Task = std::function<void()>;
	//using Task = std::function<void(void*)>;
	//using Task2 = std::function<void(void*, void*)>;

private:
	std::vector<std::thread> pool;		// �̳߳�
	std::queue<Task> tasks;				// �������
	std::mutex m_task;					// ͬ��
	std::condition_variable cv_task;
	std::atomic<bool> begin;			// �Ƿ����ύ
	std::atomic<bool> work;				// �Ƿ�ʼ����

public:
	// ����
	TaskExecutor(size_t size = 1) : begin{ true }, work{ true }
	{
		size = size < 1 ? 1 : size;
		for (size_t i = 0; i< size; ++i){
			pool.emplace_back(&TaskExecutor::schedual, this);    // push_back(std::thread{...})
		}
	}

	// ����
	~TaskExecutor()
	{
		shutdown();
		this->work.store(false);
		for (std::thread& thread : pool)
		{
			//thread.detach();    // ���̡߳���������
			thread.join();        // �ȴ���������� ǰ�᣺�߳�һ����ִ����
		}
	}

	// ֹͣ�����ύ
	void shutdown()
	{
		this->begin.store(false);
	}

	// ���������ύ
	void restart()
	{
		this->begin.store(true);
	}

	// �Ƿ�Ϊ��
	bool empty()
	{
		return tasks.empty();
	}

	int size()
	{
		return tasks.size();
	}

	// �ύһ������
	template<class F, class... Args>
	auto commit(F&& f, Args&&... args) ->std::future<decltype(f(args...))>
	{
		if (!begin.load()){
			throw std::runtime_error("task executor have closed commit.");
		}

		using ResType = decltype(f(args...));    // typename std::result_of<F(Args...)>::type, ���� f �ķ���ֵ����
		auto task = std::make_shared<std::packaged_task<ResType()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...));    // wtf !
		{    // ������񵽶���
			std::lock_guard<std::mutex> lock{ m_task };
			tasks.emplace([task](){   // push(Task{...})
				(*task)();
			});
		}
		cv_task.notify_all();    // �����߳�ִ��

		std::future<ResType> future = task->get_future();
		return future;
	}

private:
	// �������
	//void schedual()
	//{
	//	while (work.load())
	//	{
	//		Sleep(1);
	//		std::unique_lock<std::mutex> lock{ m_task };
	//		if (cv_task.wait_for(
	//			lock,
	//			std::chrono::milliseconds(10),
	//			[this](){ return !tasks.empty(); }))
	//		{
	//			Task task{ std::move(tasks.front()) };    // ȡһ�� task
	//			tasks.pop();
	//			task();
	//		}
	//	}
	//}
	// ��ȡһ����ִ�е� task
	Task get_one_task(){
		std::unique_lock<std::mutex> lock{ m_task };
		cv_task.wait(lock, [this](){ return !tasks.empty(); });    // wait ֱ���� task
		Task task{ std::move(tasks.front()) };    // ȡһ�� task
		tasks.pop();
		return task;
	}

	// �������
	void schedual(){
		while (true){
			if (Task task = get_one_task()){
				task();    //
			}
			else{
				// return;    // done
			}
		}
	}
};
