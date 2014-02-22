
#include <AudioDX/AudioPlaybackDevice.h>
#include <AudioDX/impl/AudioPlaybackDeviceImpl.h>

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

}