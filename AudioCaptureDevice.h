
#pragma once

#include <AudioDX/AbstractAudioDevice.h>

namespace AudioDX
{

    class AudioCaptureDeviceImpl;

    class AudioCaptureDevice : public AbstractAudioDevice
    {
    public:

        AudioCaptureDevice();
        virtual ~AudioCaptureDevice();

        virtual bool initialize();

        virtual bool isCaptureDevice() const;

    };

}