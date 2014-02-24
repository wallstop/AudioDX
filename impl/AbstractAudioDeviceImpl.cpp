
#include <AudioDX/impl/AbstractAudioDeviceImpl.h>

namespace AudioDX
{

#ifdef WIN32

    AbstractAudioDeviceImpl::AbstractAudioDeviceImpl() 
        : m_initialized(false), m_started(false), m_mmDevice(nullptr), m_client(nullptr)
    {
    }

    AbstractAudioDeviceImpl::AbstractAudioDeviceImpl(IMMDevice *device) 
        : m_mmDevice(device), m_client(nullptr), m_initialized(false), m_started(false)
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
        int ok = m_mmDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL,
            reinterpret_cast<void** >(&m_client));
        if(ok < 0 || !m_client)
        {
            // Unable to get an IAudioClient handle
            return false;
        }

        // TODO: Base querying time off of GetDevicePeriod(...)

        return true;
    }

    bool AbstractAudioDeviceImpl::start()
    {
        if(!m_initialized)
            initialize();
        if(m_client)
        {
            const int ok = m_client->Start();
            if(ok >= 0)
            {
                m_started = true;
                return true;
            }
        }
        return false;
    }

    bool AbstractAudioDeviceImpl::isStarted() const
    {
        return (m_client && m_started);
    }

    bool AbstractAudioDeviceImpl::stop()
    {
        if(m_client)
        {
            const int ok = m_client->Stop();
            if(ok >= 0)
            {
                m_started = false;
                return true;
            }
        }
        return false;
    }

    bool AbstractAudioDeviceImpl::isStopped() const
    {
        return (m_client && !m_started);
    }

    bool AbstractAudioDeviceImpl::isValid() const
    {
        return m_initialized && (isStarted() || isStopped());
    }

    AudioFormat AbstractAudioDeviceImpl::getAudioFormat() const
    {
        return m_audioFormat;
    }
    
#endif

}