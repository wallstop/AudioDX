
#pragma once

#include <atomic>

namespace AudioDX
{
    
    class TaskCallback
    {
    public:
        TaskCallback(bool started = true);
        virtual ~TaskCallback();

        virtual void stopTask(bool stop = true) const;
        virtual bool isTaskStopped() const;

    private:
        mutable std::atomic<bool> m_cancel;
    };

}

