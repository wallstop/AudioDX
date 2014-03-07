
#include <AudioDX/AudioCaptureDevice.h>
#include <AudioDX/impl/AudioCaptureDeviceImpl.h>
#include <AudioDX/AudioPacket.h>

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

    AudioPacket AudioCaptureDevice::readFromBuffer()
    {
        if(impl)
            return impl->readFromBuffer();
        return BAD_BUFFER;
    }

    bool AudioCaptureDevice::readFromBuffer(AudioStream& out, TaskCallback* callback)
    {
        return (impl && impl->readFromBuffer(out, callback));
    }

    bool AudioCaptureDevice::writeToBuffer(const AudioPacket& in, const AbstractFilter& filter)
    {
        return (impl && impl->writeToBuffer(in, filter));
    }

    bool AudioCaptureDevice::writeToBuffer(AudioStream& in, const AbstractFilter& filter, TaskCallback* callback)
    {
        return (impl && impl->writeToBuffer(in, filter, callback));
    }
}