
#pragma once

#include "../AbstractAudioDevice.h"
#include "../AudioFormat.h"
#include "../Tasks/TaskCallback.h"
#include "../AudioStream.h"

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

    class AudioPacket;
    struct AbstractFilter;

    class AbstractAudioDeviceImpl
    {
    public:
        AbstractAudioDeviceImpl();
        AbstractAudioDeviceImpl(IMMDevice *mmDevice);
        virtual ~AbstractAudioDeviceImpl();

        virtual bool        initialize(TaskCallback *callback = nullptr) = 0;

        // TODO: Make start() and stop() pure virtual
        virtual bool        start();
        virtual bool        isStarted() const;
        virtual bool        stop();
        virtual bool        isStopped() const;

        virtual AudioFormat getAudioFormat() const;
        virtual AudioPacket readFromBuffer() = 0;
        virtual bool        readFromBuffer(AudioStream& out, TaskCallback* callback = nullptr) = 0;
        virtual bool        writeToBuffer(const AudioPacket& in, const AbstractFilter& filter) = 0;
        virtual bool        writeToBuffer(AudioStream& in, const AbstractFilter& filter, TaskCallback* callback) = 0;

        virtual bool        isValid() const;


    protected:
        IMMDevice*          m_mmDevice;
        IAudioClient*       m_client;

        AudioFormat         m_audioFormat;

        long long           m_referenceTime;

        bool                m_initialized;
        // Having one boolean to control for isStarted or isStopped could lead to some
        // wonky states.. What about an uninitialized device? 
        bool                m_started;

        friend class AudioDeviceManagerImpl;

        };

#endif

}

