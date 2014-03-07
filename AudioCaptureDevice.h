
#pragma once

#include <AudioDX/AbstractAudioDevice.h>

namespace AudioDX
{

    class AudioCaptureDeviceImpl;
    class AudioPacket;

    class AudioCaptureDevice : public AbstractAudioDevice
    {
    public:

        AudioCaptureDevice();
        virtual ~AudioCaptureDevice();

        virtual bool        initialize();

        virtual bool        isCaptureDevice() const;

        virtual AudioPacket readFromBuffer();
        virtual bool        readFromBuffer(AudioStream& out, TaskCallback* callback);
        virtual bool        writeToBuffer(const AudioPacket& in, const AbstractFilter& filter);
        virtual bool        writeToBuffer(AudioStream& in, const AbstractFilter& filter, TaskCallback* callback);

    };

}