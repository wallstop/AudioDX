
#pragma once

#include <atomic>

class TaskableCallback
{
public:

    TaskableCallback();
    virtual ~TaskableCallback();

    virtual void updateProgress(unsigned int current, unsigned int total);
    virtual void updateProgress(unsigned int current);

    virtual void stopTask(bool stop = false);

private:
    std::atomic<unsigned int>	m_current;
    std::atomic<unsigned int>	m_total;
    std::atomic<bool>			m_cancel;
};

