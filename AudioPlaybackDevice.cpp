
#include <AudioDX/AudioPlaybackDevice.h>
#include <AudioDX/impl/AudioPlaybackDeviceImpl.h>
#include <AudioDX/AudioBuffer.h>

namespace AudioDX
{
    AudioPlaybackDevice::AudioPlaybackDevice()
    {
        impl = std::make_shared<AudioPlaybackDeviceImpl>();
    }

    AudioPlaybackDevice::~AudioPlaybackDevice()
    {
    }

    bool AudioPlaybackDevice::initialize()
    {
        return (impl && impl->initialize());
    }

    inline bool AudioPlaybackDevice::isPlaybackDevice() const
    {
        return true;
    }

    bool AudioPlaybackDevice::writeToBuffer(const AudioBuffer& in, const AbstractFilter& filter)
    {
        return (impl && impl->writeToBuffer(in, filter));
    }

    AudioBuffer AudioPlaybackDevice::readFromBuffer()
    {
        if(impl)
            return impl->readFromBuffer();
        return BAD_BUFFER;
    }

}