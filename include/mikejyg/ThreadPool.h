/*
 * ThreadPool.h
 *
 *      Author: mikejyg
 *
 *      derived from: https://github.com/progschj/ThreadPool
 *
 * mods:
 * 	 To avoid move of packaged_task, and use a shared pointer to work around the issue of
 *   	lacking lambda move capture.
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <stdexcept>
#include <future>

namespace mikejyg {

/**
 * an interface that can wrap (own) a packaged_task, so that,
 * 	it can be used without a type template
 */
class TaskWrapperIntf {
public:
	virtual ~TaskWrapperIntf() {}

	virtual void execute() = 0;

};

template<typename T>
class TaskWrapper : public TaskWrapperIntf {
public:
	using TaskType = std::packaged_task<T()>;
	using TaskUptrType = std::unique_ptr<TaskType>;

protected:
	TaskUptrType taskUptr;

public:
	virtual ~TaskWrapper() {}

	TaskWrapper(TaskUptrType & taskUptr) : taskUptr(std::move(taskUptr)) {}

	TaskWrapper(TaskType * taskUptr) : taskUptr(taskUptr) {}

	virtual void execute() override {
		(*taskUptr)();
	}

};

class ThreadPool {
public:
    ThreadPool(size_t threads) : stop(false)
	{
    	for(size_t i = 0;i<threads;++i)
    		workers.emplace_back(
    				[this]
					 {
    		for(;;)
    		{
    			TaskWrapperIntf * taskWrapperPtr;

    			{
    				std::unique_lock<std::mutex> lock(this->queue_mutex);
    				this->condition.wait(lock,
    						[this]{ return this->stop || !this->tasks.empty(); });
    				if(this->stop && this->tasks.empty())
    					return;

    				taskWrapperPtr = this->tasks.front();
    				this->tasks.pop();
    			}

    			taskWrapperPtr->execute();

    			delete taskWrapperPtr;

    		}
					 }
    		);
	}

    template<class F>
    auto enqueue(F&& f)
        -> std::future<typename std::result_of<F()>::type>;

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop = true;
        }
        condition.notify_all();
        for(std::thread &worker: workers)
            worker.join();
    }

private:

    // need to keep track of threads so we can join them
    std::vector< std::thread > workers;

    // the task queue
    std::queue< TaskWrapperIntf * > tasks;

    // synchronization
    std::mutex queue_mutex;
    std::condition_variable condition;

    bool stop;

};

// add new work item to the pool
template<class F>
auto ThreadPool::enqueue(F&& f)
    -> std::future<typename std::result_of<F()>::type>
{
    using return_type = typename std::result_of<F()>::type;

    auto taskPtr = new std::packaged_task<return_type()>(f);

    std::future<return_type> res = taskPtr->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace( new TaskWrapper<return_type>(taskPtr) );
    }
    condition.notify_one();
    return res;
}

} /* namespace mikejyg */

#endif /* THREADPOOL_H_ */
