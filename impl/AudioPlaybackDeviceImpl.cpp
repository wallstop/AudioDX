
#include <AudioDX/impl/AudioPlaybackDeviceImpl.h>
#include <AudioDX/AudioBuffer.h>
#include <AudioDX/Filters/AbstractFilter.h>

namespace AudioDX
{

#ifdef WIN32

#define REFERENCE_TIMES_PER_MILLISECOND 10000
#define REFERENCE_TIMES_PER_SECOND 1000 * REFERENCE_TIMES_PER_MILLISECOND
	
    AudioPlaybackDeviceImpl::AudioPlaybackDeviceImpl(IMMDevice* mmDevice, IAudioRenderClient* playbackClient) 
        : AbstractAudioDeviceImpl(mmDevice), m_playbackClient(playbackClient)
    {
    }

    AudioPlaybackDeviceImpl::~AudioPlaybackDeviceImpl()
    {
        stop();
        releaseDevice(m_playbackClient);
    }

    bool AudioPlaybackDeviceImpl::initialize(TaskableCallback* callback)
    {
        if(!AbstractAudioDeviceImpl::initialize(callback))
            return false;

        // Setup our wave format
        WAVEFORMATEX *waveFormat = nullptr;
        int ok = m_client->GetMixFormat(&waveFormat);
        if(ok < 0 || !waveFormat)
        {
            // Unable to get the device's audio format
            releaseDevice(m_client);
            CoTaskMemFree(waveFormat);
            return false;
        }

        m_audioFormat.channels          = waveFormat->nChannels;
        m_audioFormat.samplesPerSecond  = waveFormat->nSamplesPerSec;
        m_audioFormat.bitsPerBlock      = waveFormat->nBlockAlign;
        m_audioFormat.bitsPerSample     = waveFormat->wBitsPerSample;

        // Try to intialize our audio client
        ok = m_client->Initialize(AUDCLNT_SHAREMODE_SHARED, 0,
            0, 0, waveFormat, 0);
        CoTaskMemFree(waveFormat);  // nullptrs are ok here
        if(ok < 0)
        {
            // Unable to properly initialize IAudioClient
            releaseDevice(m_client);
            return false;
        }

        ok = m_client->GetService(__uuidof(IAudioRenderClient),
            reinterpret_cast<void** >(&m_playbackClient));
        if(ok < 0)
        {
            releaseDevice(m_client);
            releaseDevice(m_playbackClient);
            return false;
        }

        // TODO: Buffer stuff. It can wait

        // still here?
        m_initialized = true;
        return true;
	}

    AudioBuffer AudioPlaybackDeviceImpl::readFromBuffer() 
    {
        return BAD_BUFFER;
    }

    bool AudioPlaybackDeviceImpl::writeToBuffer(const AudioBuffer& in, const AbstractFilter& filter)
    {
        if(!isStarted())
        {
            // Someone called this method without starting. Let's see what we can do...
            start();
            // Now, if we're still not started, bail
            if(!isStarted())
                return false;
        }


        if(!m_initialized)
        {
            // If we're not initialized here, we can't proceed. Bail out
            return false;
        }

        if(!m_client || !m_playbackClient)
        {
            // Something's wrong with our playbackClient - we don't have one!
            return false;
        }

        AudioByte *data = nullptr;
        unsigned int sizeOfBuffer = 0;
        int ok = m_client->GetBufferSize(&sizeOfBuffer);
        if(ok < 0)
        {
            // Something went wrong with getting the buffer size (...how?)
            return false;
        }

        unsigned int bytesPadded = 0;
        ok = m_client->GetCurrentPadding(&bytesPadded);
        if(ok < 0)
        {
            // Something went wrong with getting the amount ofpadding (...how?)
            return false;
        }

        sizeOfBuffer -= bytesPadded;
        ok = m_playbackClient->GetBuffer(sizeOfBuffer, &data);
        if(ok < 0)
        {
            // Something went wrong with grabbing a pointer to the buffer. That's no good.
            m_playbackClient->ReleaseBuffer(sizeOfBuffer, 0); // Should we be releasing this here? Do we care?
            return false;
        }

        // We have to do some size calculations to get the appropriately sized buffer from whatever is passed in
        const size_t outSize = determineBufferSize(in, m_audioFormat);
        AudioBuffer myBuffer(m_audioFormat, outSize);
        const bool transformOk = filter.transformBuffer(in, myBuffer);
        unsigned int trueSize = sizeOfBuffer < outSize? sizeOfBuffer : outSize;
        if(!transformOk)
        {
            m_playbackClient->ReleaseBuffer(sizeOfBuffer, 0);
            return false;
        }

        for(unsigned int i = 0; i < trueSize
            ; ++i)
            data[i] = myBuffer.at(i);
        //memcpy(data, myBuffer.data(), trueSize);

        ok = m_playbackClient->ReleaseBuffer(sizeOfBuffer, 0);
        if(ok < 0)
        {
            // Some error occured, but by now, we don't care about it...
        }

        return true;
    }

#endif

}