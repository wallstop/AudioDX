
#include <AudioDX/AudioCaptureDevice.h>
#include <AudioDX/AudioPlaybackDevice.h>

#include <AudioDX/impl/AudioDeviceManagerImpl.h>
#include <AudioDX/impl/AudioCaptureDeviceImpl.h>
#include <AudioDX/impl/AudioPlaybackDeviceImpl.h>

namespace AudioDX
{

#ifdef WIN32

    // ONLY CALL IF CoInitialize() has already been called
    // This will clean up after itself on application exit...
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
        
        if(flowType != eRender || flowType != eCapture)
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
        std::vector<std::shared_ptr<AudioPlaybackDevice>> ret;
        for(auto deviceItr = m_devices.cbegin(); deviceItr != m_devices.cend(); ++deviceItr)
        {
            AudioPlaybackDevice* device = dynamic_cast<AudioPlaybackDevice*>(deviceItr->get());
            if(device)
                ret.push_back(std::shared_ptr<AudioPlaybackDevice>(device));
        }
        return ret;
    }

    std::vector<std::shared_ptr<AudioCaptureDevice>> AudioDeviceManagerImpl::getCaptureDevices() const
    {
        std::vector<std::shared_ptr<AudioCaptureDevice>> ret;
        for(auto deviceItr = m_devices.cbegin(); deviceItr != m_devices.cend(); ++deviceItr)
        {
            AudioCaptureDevice* device = dynamic_cast<AudioCaptureDevice*>(deviceItr->get());
            if(device)
                ret.push_back(std::shared_ptr<AudioCaptureDevice>(device));
        }
        return ret;
    }

    std::vector<std::shared_ptr<AbstractAudioDevice>> AudioDeviceManagerImpl::getAllDevices() const
    {
        return std::vector<std::shared_ptr<AbstractAudioDevice>>(m_devices.cbegin(), m_devices.cend());
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
                if(dynamic_cast<DeviceType*>(deviceItr->get()))
                {
                    m_devices.erase(deviceItr);
                    break;
                }
            }
            finished = true;
        }
    }

#endif

}
