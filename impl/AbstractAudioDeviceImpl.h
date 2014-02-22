
#pragma once

#include <AudioDX/AbstractAudioDevice.h>
#include <AudioDX/AudioFormat.h>
#include <AudioDX/Utilities/TaskableCallback.h>

#ifdef WIN32

#include <windows.h>
#include <mmsystem.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>

#endif

namespace AudioDX
{

#ifdef WIN32

// Safely releases some IUnknown* object
#define releaseDevice(device)   \
    if((device)) { (device)->Release(); (device) = nullptr; }

    class AbstractAudioDeviceImpl
    {
    public:
        AbstractAudioDeviceImpl();
        AbstractAudioDeviceImpl(IMMDevice *mmDevice);
        virtual ~AbstractAudioDeviceImpl();

        virtual bool initialize(TaskableCallback *callback = nullptr) = 0;

        virtual bool start();
        virtual bool stop();

        virtual AudioFormat getAudioFormat() const;

        virtual bool isValid() const;

    protected:
        IMMDevice* m_mmDevice;
        IAudioClient *m_client;

        AudioFormat m_audioFormat;

        bool m_initialized;

        };

#endif

}

