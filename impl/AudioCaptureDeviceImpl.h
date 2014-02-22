
#pragma once

#include <AudioDX/impl/AbstractAudioDeviceImpl.h>

namespace AudioDX
{

#ifdef WIN32

    class AudioCaptureDeviceImpl : public AbstractAudioDeviceImpl
    {
    public:

        AudioCaptureDeviceImpl(IMMDevice *mmDevice = nullptr, 
            IAudioCaptureClient* captureClient = nullptr);
        virtual ~AudioCaptureDeviceImpl();

        virtual bool initialize(TaskableCallback* callback = nullptr);

    protected:
        IAudioCaptureClient *m_captureClient;
    };

#endif

}