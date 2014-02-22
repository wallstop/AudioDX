
#include <AudioDX/Utilities/TaskableCallback.h>

#include <assert.h>

TaskableCallback::TaskableCallback() 
    : m_current(0), m_total(0), m_cancel(false)
{
}

TaskableCallback::~TaskableCallback()
{
}

void TaskableCallback::updateProgress(unsigned int current, unsigned int total)
{
    assert(current >= m_current);
    assert(current <= total);
    assert(current <= m_total);

    m_current = current;
    m_total = total;
}

void TaskableCallback::updateProgress(unsigned int current)
{
    assert(current >= m_current);
    assert(current <= m_total);

    m_current = current;
}

void TaskableCallback::stopTask(bool stop)
{
    m_cancel = stop;
}
