
#pragma once

#include <AudioDX/AbstractAudioDevice.h>
#include <AudioDX/Filters/AbstractFilter.h>

namespace AudioDX
{

    class AudioPlaybackDeviceImpl;
    class AudioBuffer;

    class AudioPlaybackDevice : public AbstractAudioDevice
    {
    public:

        AudioPlaybackDevice();
        virtual ~AudioPlaybackDevice();

        virtual bool        initialize();

        virtual bool        isPlaybackDevice() const;

        virtual bool        writeToBuffer(const AudioBuffer& in, const AbstractFilter& filter = AbstractFilter());
        virtual AudioBuffer readFromBuffer();

    };
}