#include "stdafx.h"
#include "thread_pool.h"

namespace xutil
{
    ThreadPool::ThreadPool(int num_threads /* = 4 */, int max_threads /* = 16 */)
    {
        running_ = true;
        max_threads_ = max_threads;
        AddWorkerThreads(num_threads);
    }

    ThreadPool::~ThreadPool()
    {
        running_ = false;
        task_queue_.clear();
        task_event_.notify_all();        
        threads_.join_all();
    }

    void ThreadPool::WaitAll()
    {
        while (!task_queue_.empty())
        {
            ::Sleep(10);
        }
    }

    int ThreadPool::num_pending_tasks() const
    {
        return task_queue_.unsafe_size();
    }

    int ThreadPool::num_threads() const
    {
        return threads_.size();
    }

    int ThreadPool::max_threads() const 
    {
        return max_threads_;
    }

    void ThreadPool::SetMaxThreads(int max)
    {
        max_threads_ = max;
    }

    void ThreadPool::QueueWorkItem(TaskPtr task)
    {
        task_queue_.push(task);
        task_event_.notify_one();
        
        if (num_pending_tasks() > 2 && num_threads() < max_threads_)
        {
            UniqueLock lock(mutex_);
            if (num_pending_tasks() > 2 && num_threads() < max_threads_)
            {
                AddWorkerThreads(1);
            }
        }
    }

    void ThreadPool::AddWorkerThreads(int count)
    {
        for (int i = 0; i < count; ++i)
        {
            threads_.create_thread(bind(&ThreadPool::PoolWorker, this));
        }
    }

    void ThreadPool::PoolWorker()
    {
        TaskPtr task;
        while (running_)
        {
            task_event_.wait();
            while (running_ && task_queue_.try_pop(task))
            {
                task->Run();
            }
        }
    }
}