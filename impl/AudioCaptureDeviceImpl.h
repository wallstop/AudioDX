
#pragma once

#include <AudioDX/impl/AbstractAudioDeviceImpl.h>

namespace AudioDX
{

#ifdef WIN32

    class AudioPacket;

    class AudioCaptureDeviceImpl : public AbstractAudioDeviceImpl
    {
    public:

        AudioCaptureDeviceImpl(IMMDevice *mmDevice = nullptr, 
            IAudioCaptureClient* captureClient = nullptr);
        virtual ~AudioCaptureDeviceImpl();

        virtual bool    initialize(TaskCallback* callback = nullptr);

        // TODO: Modify signature to provide error handling features
        virtual AudioPacket readFromBuffer();
        virtual bool        readFromBuffer(AudioStream& out, TaskCallback* callback);
        virtual bool        writeToBuffer(const AudioPacket& in, const AbstractFilter& filter);
        virtual bool        writeToBuffer(AudioStream& in, const AbstractFilter& filter, TaskCallback* callback);

    protected:
        IAudioCaptureClient *m_captureClient;
    };

#endif

}