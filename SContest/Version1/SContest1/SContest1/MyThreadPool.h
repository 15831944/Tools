#pragma once

#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <future>
#include <atomic>
#include <vector>
#include <queue>

//using Task = std::function<void()>;
//class TaskExecutor;
//
//bool get_one_task(Task& task);
//void schedual(TaskExecutor* exe);

class TaskExecutor
{
	using Task = std::function<void()>;
	//using Task = std::function<void(void*)>;
	//using Task2 = std::function<void(void*, void*)>;

public:
	std::vector<std::thread> pool;		// 线程池
	std::queue<Task> tasks;				// 任务队列
	std::mutex m_task;					// 同步
	//std::condition_variable cv_task;
	std::atomic<bool> begin;			// 是否开启提交
	std::atomic<bool> work;				// 是否开始工作

public:
	// 构造
	TaskExecutor(size_t size = 1) : begin{ true }, work{ true }
	{
		size = size < 1 ? 1 : size;
		for (size_t i = 0; i< size; ++i){
			pool.emplace_back(&TaskExecutor::schedual, this);    // push_back(std::thread{...})
			//pool.push_back(std::thread{ &schedual, this });
		}
	}

	// 析构
	~TaskExecutor()
	{
		shutdown();
		this->work.store(false);
		for (std::thread& thread : pool)
		{
			//thread.detach();    // 让线程“自生自灭”
			thread.join();        // 等待任务结束， 前提：线程一定会执行完
		}
	}

	// 停止任务提交
	void shutdown()
	{
		this->begin.store(false);
	}

	// 重启任务提交
	void restart()
	{
		this->begin.store(true);
	}

	// 是否为空
	bool empty()
	{
		return tasks.empty();
	}

	int size()
	{
		return tasks.size();
	}

	// 提交一个任务
	template<class F, class... Args>
	auto commit(F&& f, Args&&... args) ->std::future<decltype(f(args...))>
	{
		if (!begin.load()){
			throw std::runtime_error("task executor have closed commit.");
		}

		using ResType = decltype(f(args...));    // typename std::result_of<F(Args...)>::type, 函数 f 的返回值类型
		auto task = std::make_shared<std::packaged_task<ResType()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...));
		{    // 添加任务到队列
			std::lock_guard<std::mutex> lock{ m_task };
			tasks.emplace([task](){   // push(Task{...})
				(*task)();
			});
		}
		//cv_task.notify_all();    // 唤醒线程执行

		std::future<ResType> future = task->get_future();
		return future;
	}

private:
	// 任务调度
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
	//			Task task{ std::move(tasks.front()) };    // 取一个 task
	//			tasks.pop();
	//			task();
	//		}
	//	}
	//}
	// 获取一个待执行的 task
	//Task get_one_task(){
	//	std::unique_lock<std::mutex> lock{ m_task };
	//	cv_task.wait(lock, [this](){ return !tasks.empty(); });    // wait 直到有 task
	//	Task task{ std::move(tasks.front()) };    // 取一个 task
	//	tasks.pop();
	//	return task;
	//}
	bool get_one_task(Task& task)
	{
		std::unique_lock<std::mutex> lock{ m_task };
		//if (!cv_task.wait_for(lock, std::chrono::milliseconds(10), [this](){ return !tasks.empty(); }))
		if (tasks.empty())
			return false;    // wait 直到有 task
		task = std::move(tasks.front());    // 取一个 task
		tasks.pop();
		return true;
	}

	//// 任务调度
	void schedual()
	{
		while (this->work)
		{
			//std::unique_lock<std::mutex> lock{ m_task };
			//m_task.lock();
			//cv_task.wait(lock, [this](){ return !tasks.empty(); });    // wait 直到有 task
			//Task task{ std::move(tasks.front()) };    // 取一个 task
			//tasks.pop();
			//task();
			//m_task.unlock();
			Task k;
			if (get_one_task(k))
				k();
			Sleep(1);
			//if (Task task = get_one_task()){
			//	task();    //
			//}
			//else{
			//	// return;    // done
			//}
		}
	}
};
