#pragma once

#include <concurrent_queue.h>

#include "primitives.h"
#include "event.h"
#include "task.h"

namespace xutil
{
    class ThreadPool
    {
    public:
        ThreadPool(int num_threads = 4, int max_threads = 16);
        ~ThreadPool();
        void QueueWorkItem(TaskPtr task);

        void WaitAll();
        int num_pending_tasks() const;
        int num_threads() const;
        int max_threads() const;
        void SetMaxThreads(int max);

        template <typename F>
        void QueueWorkItem(F f)
        {
            return QueueWorkItem(CreateTask(f));
        }

        template <typename F, typename A1>
        void QueueWorkItem(F f, A1 a1)
        {
            return QueueWorkItem(CreateTask(bind(f, a1)));
        }

        template <typename F, typename A1, typename A2>
        void QueueWorkItem(F f, A1 a1, A2 a2)
        {
            return QueueWorkItem(CreateTask(bind(f, a1, a2)));
        }

        template <typename F, typename A1, typename A2, typename A3>
        void QueueWorkItem(F f, A1 a1, A2 a2, A3 a3)
        {
            return QueueWorkItem(CreateTask(bind(f, a1, a2, a3)));
        }

        template <typename F, typename A1, typename A2, typename A3, typename A4>
        void QueueWorkItem(F f, A1 a1, A2 a2, A3 a3, A4 a4)
        {
            return QueueWorkItem(CreateTask(bind(f, a1, a2, a3, a4)));
        }

    private:
        void PoolWorker();
        void AddWorkerThreads(int total_count);

        Mutex mutex_;
        volatile int max_threads_;
        ThreadGroup threads_;

        volatile bool running_;
        Event task_event_;
        Concurrency::concurrent_queue<TaskPtr> task_queue_;
    };
}