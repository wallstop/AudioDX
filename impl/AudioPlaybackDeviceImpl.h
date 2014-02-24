
#pragma once

#include <AudioDX/impl/AbstractAudioDeviceImpl.h>

namespace AudioDX
{

#ifdef WIN32

    struct AbstractFilter;

    class AudioPlaybackDeviceImpl : public AbstractAudioDeviceImpl
    {
    public:

        AudioPlaybackDeviceImpl(IMMDevice* mmDevice = nullptr, IAudioRenderClient* playbackClient = nullptr);
        virtual ~AudioPlaybackDeviceImpl();

        virtual bool        initialize(TaskableCallback* callback = nullptr);

        // TODO: Modify signature to provide error handling features
        virtual bool        writeToBuffer(const AudioBuffer& in, const AbstractFilter& filter);
        virtual AudioBuffer readFromBuffer();

    protected:
        IAudioRenderClient*	m_playbackClient;

    };

#endif

}