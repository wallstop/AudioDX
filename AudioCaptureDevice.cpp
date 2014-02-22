
#include <AudioDX/AudioCaptureDevice.h>
#include <AudioDX/impl/AudioCaptureDeviceImpl.h>

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

}