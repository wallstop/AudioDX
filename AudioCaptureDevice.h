
#pragma once

#include <AudioDX/AbstractAudioDevice.h>

namespace AudioDX
{

    class AudioCaptureDeviceImpl;
    class AudioBuffer;

    class AudioCaptureDevice : public AbstractAudioDevice
    {
    public:

        AudioCaptureDevice();
        virtual ~AudioCaptureDevice();

        virtual bool        initialize();

        virtual bool        isCaptureDevice() const;

        virtual AudioBuffer readFromBuffer();
        virtual bool        writeToBuffer(const AudioBuffer& in, const AbstractFilter& filter);

    };

}