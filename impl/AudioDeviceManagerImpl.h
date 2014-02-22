
#pragma once

#include <AudioDX/AudioDeviceManager.h>

#include <set>
#include <atomic>

#ifdef WIN32

#include <mmdeviceapi.h>

#endif

namespace AudioDX
{

#ifdef WIN32

	class AbstractAudioDevice;
    struct EDataFlow;

	class AudioDeviceManagerImpl
	{
	public:
		AudioDeviceManagerImpl();
		~AudioDeviceManagerImpl();

		bool initialize();
        bool unInitialize();

		bool isInitialized() const;

        void enumerateAllDevices();
        void enumeratePlaybackDevices();
        void enumerateCaptureDevices();

        std::vector<std::shared_ptr<AudioCaptureDevice>>    getCaptureDevices() const;
        std::vector<std::shared_ptr<AudioPlaybackDevice>>   getPlaybackDevices() const;        
        std::vector<std::shared_ptr<AbstractAudioDevice>>   getAllDevices() const;
        //std::shared_ptr<AudioPlaybackDevice>                getDefaultPlaybackDevice() const;

	private:

        // This is some tight coupling of the template arguments...
        template <typename DeviceType, typename DeviceTypeImpl, ::EDataFlow flowType>
        void enumerateDevicesOfType();

        template <typename DeviceType>
        void removeDevicesOfType();

        void removeDevice(std::shared_ptr<AbstractAudioDevice> device);

		std::atomic<bool> m_initialized;
		std::set<std::shared_ptr<AbstractAudioDevice>> m_devices;
	};

#endif

}