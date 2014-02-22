
#include <AudioDX/Tasks/AudioSilenceTask.h>

namespace AudioDX
{

    AudioSilenceTask::AudioSilenceTask()
    {
    }

    AudioSilenceTask::~AudioSilenceTask()
    {
    }

    void AudioSilenceTask::run(TaskableCallback *callback)
    {
        if(!m_device)
            return;

        // TODO: STUB
    }

}