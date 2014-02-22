
#include <AudioDX/impl/AbstractAudioDeviceImpl.h>

namespace AudioDX
{

#ifdef WIN32

    AbstractAudioDeviceImpl::AbstractAudioDeviceImpl() 
        : m_initialized(false), m_mmDevice(nullptr), m_client(nullptr)
    {
    }

    AbstractAudioDeviceImpl::AbstractAudioDeviceImpl(IMMDevice *device) 
        : m_mmDevice(device), m_client(nullptr), m_initialized(false)
    {
    }

    AbstractAudioDeviceImpl::~AbstractAudioDeviceImpl()
    {
        stop();        
        releaseDevice(m_client);
        releaseDevice(m_mmDevice);
    }

    bool AbstractAudioDeviceImpl::initialize(TaskableCallback* callback)
    {
        // Make sure we only initialize once
		if(!m_mmDevice || m_initialized)
		{
			// Need an mmDevice handle to proceed
			return false;
		}

        // Grab our AudioClient
        int ok = m_mmDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr,
            reinterpret_cast<void** >(&m_client));
        if(ok < 0 || !m_client)
        {
        // Unable to get an IAudioClient handle
        return false;
        }

        return true;
    }

    bool AbstractAudioDeviceImpl::start()
    {
        return (m_client && (m_client->Start() < 0 ? false : true));
    }

    bool AbstractAudioDeviceImpl::stop()
    {
        return (m_client && (m_client->Stop() < 0 ? false : true));
    }

    bool AbstractAudioDeviceImpl::isValid() const
    {
        return m_initialized;
    }

    AudioFormat AbstractAudioDeviceImpl::getAudioFormat() const
    {
        return m_audioFormat;
    }
    
#endif

}