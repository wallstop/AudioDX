
#include <AudioDX/AudioDeviceManager.h>
#include <AudioDX/impl/AudioDeviceManagerImpl.h>

#include <AudioDX/AudioCaptureDevice.h>
#include <AudioDX/AudioPlaybackDevice.h>

namespace AudioDX
{
	AudioDeviceManager::AudioDeviceManager()
	{
        impl = std::make_shared<AudioDeviceManagerImpl>();
	}

	AudioDeviceManager::~AudioDeviceManager()
	{
	}

	bool AudioDeviceManager::initialize()
	{
		return (impl && impl->initialize());
	}

    bool AudioDeviceManager::unInitialize()
    {
        return (impl && impl->unInitialize());
    }

	bool AudioDeviceManager::isInitialized() const
	{
		return (impl && impl->isInitialized());
	}
    
    std::vector<std::shared_ptr<AudioPlaybackDevice>> AudioDeviceManager::getPlaybackDevices() const
    {
        if(impl)
            return impl->getPlaybackDevices();
        return std::vector<std::shared_ptr<AudioPlaybackDevice>>();
    }

    std::vector<std::shared_ptr<AudioCaptureDevice>> AudioDeviceManager::getCaptureDevices() const
    {
        if(impl)
            return impl->getCaptureDevices();
        return std::vector<std::shared_ptr<AudioCaptureDevice>>();
    }

    std::vector<std::shared_ptr<AbstractAudioDevice>> AudioDeviceManager::getAllDevices() const
    {
        if(impl)
            return impl->getAllDevices();
        return std::vector<std::shared_ptr<AbstractAudioDevice>>();
    }

}
