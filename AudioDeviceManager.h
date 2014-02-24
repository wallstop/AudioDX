
#pragma once

#include <vector>
#include <memory>

namespace AudioDX
{

    class AudioDeviceManagerImpl;
    class AbstractAudioDevice;
    class AudioCaptureDevice;
    class AudioPlaybackDevice;

    class AudioDeviceManager
    {
    public:
        AudioDeviceManager();
        ~AudioDeviceManager();

        bool initialize();
        bool unInitialize();

        std::vector<std::shared_ptr<AudioPlaybackDevice>>   getPlaybackDevices() const;
        std::vector<std::shared_ptr<AudioCaptureDevice>>    getCaptureDevices() const;
        std::vector<std::shared_ptr<AbstractAudioDevice>>   getAllDevices() const;
        std::shared_ptr<AudioCaptureDevice>                 getDefaultCaptureDevice() const;
        std::shared_ptr<AudioPlaybackDevice>                getDefaultPlaybackDevice() const;

        bool isInitialized() const;

    private:

        std::shared_ptr<AudioDeviceManagerImpl> impl;
	};

}