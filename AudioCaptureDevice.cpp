
#include <AudioDX/AudioCaptureDevice.h>
#include <AudioDX/impl/AudioCaptureDeviceImpl.h>
#include <AudioDX/AudioBuffer.h>

namespace AudioDX
{
    AudioCaptureDevice::AudioCaptureDevice()
    {
        impl = std::make_shared<AudioCaptureDeviceImpl>();
    }

    AudioCaptureDevice::~AudioCaptureDevice()
    {
    }

    bool AudioCaptureDevice::initialize()
    {
        return (impl && impl->initialize());
    }

    inline bool AudioCaptureDevice::isCaptureDevice() const
    {
        return true;
    }

    AudioBuffer AudioCaptureDevice::readFromBuffer()
    {
        if(impl)
            return impl->readFromBuffer();
        return BAD_BUFFER;
    }

    bool AudioCaptureDevice::writeToBuffer(const AudioBuffer& in, const AbstractFilter& filter)
    {
        return (impl && impl->writeToBuffer(in, filter));
    }

}