
#include <AudioDX/Tasks/TaskCallback.h>

#include <assert.h>

namespace AudioDX
{

    TaskCallback::TaskCallback(bool started) :  m_cancel(!started)
    {
    }

    TaskCallback::~TaskCallback()
    {
    }

    void TaskCallback::stopTask(bool stop) const
    {
        m_cancel = stop;
    }

    bool TaskCallback::isTaskStopped() const
    {
        return m_cancel;
    }

}
