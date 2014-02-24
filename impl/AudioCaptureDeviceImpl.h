
#pragma once

#include <AudioDX/impl/AbstractAudioDeviceImpl.h>

namespace AudioDX
{

#ifdef WIN32

    class AudioBuffer;

    class AudioCaptureDeviceImpl : public AbstractAudioDeviceImpl
    {
    public:

        AudioCaptureDeviceImpl(IMMDevice *mmDevice = nullptr, 
            IAudioCaptureClient* captureClient = nullptr);
        virtual ~AudioCaptureDeviceImpl();

        virtual bool    initialize(TaskableCallback* callback = nullptr);

        // TODO: Modify signature to provide error handling features
        virtual AudioBuffer     readFromBuffer();
        virtual bool            writeToBuffer(const AudioBuffer& in, const AbstractFilter& filter);

    protected:
        IAudioCaptureClient *m_captureClient;
    };

#endif

}