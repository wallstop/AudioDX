
#pragma once

#include <AudioDX/AbstractAudioDevice.h>

namespace AudioDX
{

    class AudioPlaybackDeviceImpl;
    class AudioPacket;

    class AudioPlaybackDevice : public AbstractAudioDevice
    {
    public:

        AudioPlaybackDevice();
        virtual ~AudioPlaybackDevice();

        virtual bool        initialize();

        virtual bool        isPlaybackDevice() const;

        virtual bool        writeToBuffer(const AudioPacket& in, const AbstractFilter& filter = AbstractFilter());
        virtual bool        writeToBuffer(AudioStream& in, const AbstractFilter& filter = AbstractFilter(), TaskCallback* callback = nullptr);

        virtual AudioPacket readFromBuffer();
        virtual bool        readFromBuffer(AudioStream& out, TaskCallback* callback = nullptr);
    };
}