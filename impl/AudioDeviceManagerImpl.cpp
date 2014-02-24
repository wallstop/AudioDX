
#include <AudioDX/AudioCaptureDevice.h>
#include <AudioDX/AudioPlaybackDevice.h>

#include <AudioDX/impl/AudioDeviceManagerImpl.h>
#include <AudioDX/impl/AudioCaptureDeviceImpl.h>
#include <AudioDX/impl/AudioPlaybackDeviceImpl.h>

namespace AudioDX
{

#ifdef WIN32

    // ONLY CALL IF CoInitialize() has already been called
    // This will clean up after itself on application exit
    static IMMDeviceEnumerator* getDeviceEnumeratorSingleton()
    {
        static IMMDeviceEnumerator* enumerator = nullptr;
        if(!enumerator)
        {
            int ok = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr,
                CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void** >(&enumerator));
            if(ok < 0)
            {
                releaseDevice(enumerator);
            }
        }
        return enumerator;
    }

    AudioDeviceManagerImpl::AudioDeviceManagerImpl() : m_initialized(false)
    {
        initialize();
    }

    AudioDeviceManagerImpl::~AudioDeviceManagerImpl()
    {
        unInitialize();
    }


    bool AudioDeviceManagerImpl::initialize()
    {
        if(!m_initialized)
        {
            int result = 0;
            result = CoInitialize(nullptr);
            m_initialized = result >= 0;

            enumerateAllDevices();
        }
		return m_initialized;
	}

    bool AudioDeviceManagerImpl::unInitialize()
    {
        if(m_initialized)
        {
            m_devices.clear();
            CoUninitialize();
            m_initialized = false;
        }
        return true;
    }

    bool AudioDeviceManagerImpl::isInitialized() const
    {
        return m_initialized;
    }

    template <typename DeviceType, typename DeviceTypeImpl, ::EDataFlow flowType>
    void AudioDeviceManagerImpl::enumerateDevicesOfType()
    {
        IMMDeviceEnumerator* deviceEnumerator = getDeviceEnumeratorSingleton();
        if(!deviceEnumerator)
        {
            // Unable to get a handle on the device enumerator
            return;
        }
        
        if(flowType != eRender && flowType != eCapture)
        {
            // Currently we only want to deal with either render OR capture devices, not everything
            return;
        }

        IMMDeviceCollection* deviceCollection = nullptr;
        int ok = deviceEnumerator->EnumAudioEndpoints(flowType, DEVICE_STATE_ACTIVE, &deviceCollection);
        if(ok < 0 || !deviceCollection)
        {
            releaseDevice(deviceCollection);
            return;
        }

        // Clear out everything we know about - we're starting fresh
        removeDevicesOfType<DeviceType>();
        
        unsigned int totalDevices = 0;
        ok = deviceCollection->GetCount(&totalDevices);
        if(ok < 0)
        {
            releaseDevice(deviceCollection);
            return;
        }
        for(unsigned int i = 0; i < totalDevices; ++i)
        {
            IMMDevice* currentDevice = nullptr;
            ok = deviceCollection->Item(i, &currentDevice);
            if(ok < 0 || !currentDevice)
            {
                releaseDevice(currentDevice);
                continue;
            }
            std::shared_ptr<DeviceTypeImpl> captureImpl = std::make_shared<DeviceTypeImpl>(currentDevice);
            std::shared_ptr<DeviceType> captureDevice   = std::make_shared<DeviceType>();
            captureDevice->impl = captureImpl;
            m_devices.insert(captureDevice);
        }
    }

    void AudioDeviceManagerImpl::enumeratePlaybackDevices()
    {
        enumerateDevicesOfType<AudioPlaybackDevice, AudioPlaybackDeviceImpl, eRender>();
    }

    void AudioDeviceManagerImpl::enumerateCaptureDevices()
    {
        enumerateDevicesOfType<AudioCaptureDevice, AudioCaptureDeviceImpl, eCapture>();
    }

    void AudioDeviceManagerImpl::enumerateAllDevices()
    {
        m_devices.clear();
        enumeratePlaybackDevices();
        enumerateCaptureDevices();  
    }

    std::vector<std::shared_ptr<AudioPlaybackDevice>> AudioDeviceManagerImpl::getPlaybackDevices() const
    {
        return getDevicesOfType<AudioPlaybackDevice>();
    }

    std::vector<std::shared_ptr<AudioCaptureDevice>> AudioDeviceManagerImpl::getCaptureDevices() const
    {
        return getDevicesOfType<AudioCaptureDevice>();
    }

    std::vector<std::shared_ptr<AbstractAudioDevice>> AudioDeviceManagerImpl::getAllDevices() const
    {
        return std::vector<std::shared_ptr<AbstractAudioDevice>>(m_devices.cbegin(), m_devices.cend());
    }

    std::shared_ptr<AudioCaptureDevice> AudioDeviceManagerImpl::getDefaultCaptureDevice() const
    {
        return getDefaultDeviceOfType<AudioCaptureDevice, AudioCaptureDeviceImpl, eCapture, eMultimedia>();
    }

    std::shared_ptr<AudioPlaybackDevice> AudioDeviceManagerImpl::getDefaultPlaybackDevice() const
    {
        return getDefaultDeviceOfType<AudioPlaybackDevice, AudioPlaybackDeviceImpl, eRender, eConsole>();
    }

    template <typename DeviceType>
    void AudioDeviceManagerImpl::removeDevicesOfType()
    {
        bool finished = false;

        while(!finished)
        {
            // One at a time... there's got to be a better way
            for(auto deviceItr = m_devices.cbegin(); deviceItr != m_devices.cend(); ++deviceItr)
            {
                if(std::dynamic_pointer_cast<DeviceType>(*deviceItr))
                {
                    m_devices.erase(deviceItr);
                    break;
                }
            }
            finished = true;
        }
    }

    template <class DeviceType>
    std::vector<std::shared_ptr<DeviceType>> AudioDeviceManagerImpl::getDevicesOfType() const
    {
        std::vector<std::shared_ptr<DeviceType>> ret;
        for(auto deviceItr = m_devices.cbegin(); deviceItr != m_devices.cend(); ++deviceItr)
        {
            std::shared_ptr<DeviceType> device = std::dynamic_pointer_cast<DeviceType>(*deviceItr);
            if(device)
                ret.push_back(std::shared_ptr<DeviceType>(device));
        }
        return ret;
    }

    template <typename DeviceType, typename DeviceTypeImpl, ::EDataFlow flowType, ::ERole role>
    std::shared_ptr<DeviceType> AudioDeviceManagerImpl::getDefaultDeviceOfType() const
    {
        if(flowType != eRender && flowType != eCapture)
            return std::shared_ptr<DeviceType>();

        IMMDeviceEnumerator* deviceEnumerator = getDeviceEnumeratorSingleton();
        if(!deviceEnumerator)
            return std::shared_ptr<DeviceType>();

        IMMDevice *device = nullptr;
        int ok = deviceEnumerator->GetDefaultAudioEndpoint(flowType, role, &device);
        if(ok < 0 || !device)
        {
            // Couldn't get the default playback device for some reason
            releaseDevice(device);
            return std::shared_ptr<DeviceType>();
        }

        //// So now we check if we have a device with a handle to that particular IMMDevice
        //for(auto deviceItr = m_devices.cbegin(); deviceItr != m_devices.cend(); ++deviceItr)
        //{
        //    // Lots of safety checks here
        //    if((*deviceItr) && (*deviceItr)->impl && ((*deviceItr)->impl->m_mmDevice == device))
        //    {
        //        // Make sure we're not returning a handle to the wrong kind of device
        //        auto ret = std::dynamic_pointer_cast<DeviceType>(*deviceItr);
        //        if(ret)
        //            return ret;
        //        // If we get here, we *DO* want to continue - we might have a good handle elsewhere
        //    }
        //}

        // If we haven't found it in our set (why isn't it there? That's weird), let's make it
        std::shared_ptr<DeviceTypeImpl> defaultDeviceImpl   = std::make_shared<DeviceTypeImpl>(device);
        std::shared_ptr<DeviceType> defaultDevice           = std::make_shared<DeviceType>();
        defaultDevice->impl = defaultDeviceImpl;

        return defaultDevice;
    }

#endif

}
