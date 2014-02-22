
#include <AudioDX/AbstractAudioDevice.h>
#include <AudioDX/impl/AbstractAudioDeviceImpl.h>

namespace AudioDX
{

    AbstractAudioDevice::AbstractAudioDevice() : impl(nullptr)
    {
    }

    AbstractAudioDevice::~AbstractAudioDevice()
    {
    }

    bool AbstractAudioDevice::initialize()
    {
        return (impl && impl->initialize());
    }

    bool AbstractAudioDevice::start()
    {
        return (impl && impl->start());
    }

    bool AbstractAudioDevice::stop()
    {
        return (impl && impl->stop());
    }

    inline bool AbstractAudioDevice::isCaptureDevice() const
    {
        return false;
    }

    inline bool AbstractAudioDevice::isPlaybackDevice() const
    {
        return false;
    }

    bool AbstractAudioDevice::isValid() const
    {
        return (impl && impl->isValid());
    }

    AudioFormat AbstractAudioDevice::getAudioFormat() const
    {
        if(impl)
            return impl->getAudioFormat();
        return AudioFormat();
    }

}