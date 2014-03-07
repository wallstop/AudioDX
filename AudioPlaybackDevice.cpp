
#include <AudioDX/AudioPlaybackDevice.h>
#include <AudioDX/impl/AudioPlaybackDeviceImpl.h>
#include <AudioDX/AudioPacket.h>

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

    bool AudioPlaybackDevice::writeToBuffer(const AudioPacket& in, const AbstractFilter& filter)
    {
        return (impl && impl->writeToBuffer(in, filter));
    }

    bool AudioPlaybackDevice::writeToBuffer(AudioStream& in, const AbstractFilter& filter, TaskCallback* callback)
    {
        return (impl && impl->writeToBuffer(in, filter, callback));
    }

    AudioPacket AudioPlaybackDevice::readFromBuffer()
    {
        if(impl)
            return impl->readFromBuffer();
        return BAD_BUFFER;
    }

    bool        AudioPlaybackDevice::readFromBuffer(AudioStream& out, TaskCallback* callback)
    {
        return (impl && impl->readFromBuffer(out, callback));
    }

}