
#include <AudioDX/impl/AudioCaptureDeviceImpl.h>
#include <AudioDX/AudioBuffer.h>

namespace AudioDX
{
	
#ifdef WIN32

    AudioCaptureDeviceImpl::AudioCaptureDeviceImpl(IMMDevice *mmDevice, IAudioCaptureClient* captureClient) 
        : AbstractAudioDeviceImpl(mmDevice), m_captureClient(captureClient)
    {
    }

    AudioCaptureDeviceImpl::~AudioCaptureDeviceImpl()
    {
        stop();
        releaseDevice(m_captureClient);
    }

    bool AudioCaptureDeviceImpl::initialize(TaskableCallback* callback)
    {
        if(!AbstractAudioDeviceImpl::initialize(callback))
            return false;

        // Setup our wave format
        WAVEFORMATEX *waveFormat = nullptr;
        int ok = m_client->GetMixFormat(&waveFormat);
		if(ok < 0 || !waveFormat)
        {
            // Unable to get the device's audio format
            //releaseDevice(m_client);
            CoTaskMemFree(waveFormat);
            return false;
        }

        m_audioFormat.channels			= waveFormat->nChannels;
        m_audioFormat.samplesPerSecond	= waveFormat->nSamplesPerSec;
        m_audioFormat.bitsPerBlock		= waveFormat->nBlockAlign;
        m_audioFormat.bitsPerSample		= waveFormat->wBitsPerSample;

        // Try to intialize our audio client
        ok = m_client->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK,
            0, 0, waveFormat, 0);
        CoTaskMemFree(waveFormat);  // nullptrs are ok here
        if(ok < 0)
        {
            // Unable to properly initialize IAudioClient
            return false;
        }

        // Try and actually get a handle to the captureClient
        ok = m_client->GetService(__uuidof(IAudioCaptureClient),
            reinterpret_cast<void** >(&m_captureClient));
        if(ok < 0 || !m_captureClient)
        {
            // Unable to properly active our captureClient
            releaseDevice(m_captureClient);
            return false;
        }

        // still here? 
        m_initialized = true;
        return true;
    }

    bool AudioCaptureDeviceImpl::writeToBuffer(const AudioBuffer& in, const AbstractFilter& filter)
    {
        return false;
    }

    AudioBuffer AudioCaptureDeviceImpl::readFromBuffer()
    {
        if(!isStarted())
        {
            start();
            if(!isStarted())
                return BAD_BUFFER;
        }

        if(!m_captureClient)
            return BAD_BUFFER;

        // See if there's any data we can get
        unsigned int numFramesToRead = 0;
        int ok = m_captureClient->GetNextPacketSize(&numFramesToRead);
        //int ok = m_captureClient->GetNextPacketSize(&numFramesToRead);
        if(ok < 0)
        {
            // Unable to get the next packet size... something went wrong
            return BAD_BUFFER;
        }

        if(numFramesToRead == 0)
        {
            return AudioBuffer(m_audioFormat, 0);
        }

        AudioByte* data = nullptr;        
        unsigned long returnFlags = 0;
        ok = m_captureClient->GetBuffer(&data, &numFramesToRead, &returnFlags, nullptr, nullptr);
        if(ok < 0)
        {
            // Something bad happened when we tried to grab the buffer
            // Releasing the buffer here may or may not be bad...
            stop();
            m_captureClient->ReleaseBuffer(numFramesToRead);
            return BAD_BUFFER;
        }

        // TODO: Some logic / error handling based off of flag return values. Skipping that for now.
        AudioBuffer ret(m_audioFormat, numFramesToRead * m_audioFormat.bitsPerBlock);

        // TODO: Think about adding some kind of bulk-inserter?
        for(size_t i = 0; i < numFramesToRead * m_audioFormat.bitsPerBlock; ++i)
            ret[i] = data[i];

        ok = m_captureClient->ReleaseBuffer(numFramesToRead);
        if(ok < 0)
        {
            // Seriously? It failed here? How?
            stop();
            bool breakPoint = false;
        }

        return ret;
    }

#endif

}
