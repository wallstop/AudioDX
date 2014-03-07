
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

        virtual bool        initialize(TaskCallback* callback = nullptr);

        // TODO: Modify signature to provide error handling features
        virtual bool        writeToBuffer(const AudioPacket& in, const AbstractFilter& filter);
        virtual bool        writeToBuffer(AudioStream& in, const AbstractFilter& filter, TaskCallback* callback);
        virtual AudioPacket readFromBuffer();
        virtual bool        readFromBuffer(AudioStream& out, TaskCallback* callback);

    protected:
        IAudioRenderClient*	m_playbackClient;

    };

#endif

}