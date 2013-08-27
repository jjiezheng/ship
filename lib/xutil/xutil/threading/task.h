#pragma once

namespace xutil
{
    class Task
    {
    public:
        virtual ~Task() = 0 { }
        virtual void Run() = 0;
    };

    typedef shared_ptr<Task> TaskPtr;

    template <typename F>
    class FunctionTask : public Task
    {
    public:
        FunctionTask(F f)
            : f_(f)
        {
        }

        virtual ~FunctionTask()
        {
        }

        virtual void Run()
        {
            f_();
        }

    private:
        F f_;
    };

    template <typename F>
    TaskPtr CreateTask(F f)
    {
        return make_shared<FunctionTask<F>>(f);
    }
}