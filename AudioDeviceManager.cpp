
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
        unInitialize();
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

    std::shared_ptr<AudioCaptureDevice> AudioDeviceManager::getDefaultCaptureDevice() const
    {
        if(impl)
            return impl->getDefaultCaptureDevice();
        return std::shared_ptr<AudioCaptureDevice>();
    }

    std::shared_ptr<AudioPlaybackDevice> AudioDeviceManager::getDefaultPlaybackDevice() const
    {
        if(impl)
            return impl->getDefaultPlaybackDevice();
        return std::shared_ptr<AudioPlaybackDevice>();
    }

    std::shared_ptr<AudioCaptureDevice> AudioDeviceManager::getDefaultPlaybackDeviceAsCaptureDevice() const
    {
        if(impl)
            return impl->getDefaultPlaybackDeviceAsCaptureDevice();
        return std::shared_ptr<AudioCaptureDevice>();
    }

}
