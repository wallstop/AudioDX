
#pragma once

#include <AudioDX/AudioDeviceManager.h>

#include <set>

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
        std::shared_ptr<AudioCaptureDevice>                 getDefaultCaptureDevice() const;
        std::shared_ptr<AudioPlaybackDevice>                getDefaultPlaybackDevice() const;
        std::shared_ptr<AudioCaptureDevice>                 getDefaultPlaybackDeviceAsCaptureDevice() const;
        std::shared_ptr<AudioCaptureDevice>                 getPlaybackDeviceAsCaptureDevice(std::shared_ptr<AudioPlaybackDevice>&) const;

	private:

        template <typename DeviceType, typename DeviceTypeImpl, ::EDataFlow flowType>
        void enumerateDevicesOfType();

        template <typename DeviceType>
        void removeDevicesOfType();

        template <typename DeviceType>
        std::vector<std::shared_ptr<DeviceType>> getDevicesOfType() const;

        template <typename DeviceType, typename DeviceTypeImpl, ::EDataFlow flowType, ::ERole role>
        std::shared_ptr<DeviceType> getDefaultDeviceOfType(int deviceMode = 0) const;

        void removeDevice(std::shared_ptr<AbstractAudioDevice> device);

        bool m_initialized;
        std::set<std::shared_ptr<AbstractAudioDevice>> m_devices;
    };

#endif

}
