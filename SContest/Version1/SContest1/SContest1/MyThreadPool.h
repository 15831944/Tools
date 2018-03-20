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

public:
	std::vector<std::thread> pool;		// Thread pool
	std::queue<Task> tasks;				// Task queue
	std::mutex m_task;					// Async lock
	std::atomic<bool> begin;			// Start to accept task
	std::atomic<bool> work;				// Start working

public:
	TaskExecutor(size_t size = 1) : begin{ true }, work{ true }
	{
		size = size < 1 ? 1 : size;
		for (size_t i = 0; i< size; ++i){
			pool.emplace_back(&TaskExecutor::schedual, this);
		}
	}

	~TaskExecutor()
	{
		shutdown();
		this->work.store(false);
		for (std::thread& thread : pool)
		{
			thread.join();				// waiting for task done, base: Taks must can be done
		}
	}

	// stop accept task
	void shutdown()	{ this->begin.store(false); }

	// start accept task
	void restart() { this->begin.store(true); }

	// if task empty
	bool empty() { return tasks.empty(); }

	// tasks count
	int size() { return tasks.size(); }

	// accept one task, all kinds of methods
	template<class F, class... Args>
	auto commit(F&& f, Args&&... args) ->std::future<decltype(f(args...))>
	{
		if (!begin.load()){
			throw std::runtime_error("task executor have closed commit.");
		}

		using ResType = decltype(f(args...));    // typename std::result_of<F(Args...)>::type, the return type of function
		auto task = std::make_shared<std::packaged_task<ResType()>>(
			std::bind(std::forward<F>(f), std::forward<Args>(args)...));
		{   // add task to queue
			std::lock_guard<std::mutex> lock{ m_task };
			tasks.emplace([task](){   // push(Task{...})
				(*task)();
			});
		}

		std::future<ResType> future = task->get_future();
		return future;
	}

private:
	bool get_one_task(Task& task)
	{
		std::unique_lock<std::mutex> lock{ m_task };
		if (tasks.empty())
			return false;
		task = std::move(tasks.front());    // pick up one task
		tasks.pop();
		return true;
	}

	// thread pool manager memthod
	void schedual()
	{
		while (this->work)
		{
			Task k;
			if (get_one_task(k))
				k();
			::Sleep(1);
		}
	}
};
