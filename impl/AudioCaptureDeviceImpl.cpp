
#include <AudioDX/impl/AudioCaptureDeviceImpl.h>
#include <AudioDX/AudioPacket.h>

#include <thread>

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

    bool AudioCaptureDeviceImpl::initialize()
    {
        if(!AbstractAudioDeviceImpl::initialize())
            return false;

        m_captureClient = nullptr;

        // Try and actually get a handle to the captureClient
        int ok = m_client->GetService(__uuidof(IAudioCaptureClient),
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

    bool AudioCaptureDeviceImpl::writeToBuffer(const AudioPacket& in, const AbstractFilter& filter)
    {
        return false;
    }

    bool AudioCaptureDeviceImpl::writeToBuffer(AudioStream& in, const AbstractFilter& filter, TaskCallback* callback)
    {
        return false;
    }

    AudioPacket AudioCaptureDeviceImpl::readFromBuffer()
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
        int ok =  0;
        ok = m_captureClient->GetNextPacketSize(&numFramesToRead);
        //int ok = m_captureClient->GetNextPacketSize(&numFramesToRead);
        if(ok < 0)
        {
            // Unable to get the next packet size... something went wrong
            return BAD_BUFFER;
        }

        if(numFramesToRead == 0)
        {
            return AudioPacket(m_audioFormat, 0);
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
        const size_t numBytesToWrite = numFramesToRead * m_audioFormat.bitsPerBlock;
        AudioPacket ret(m_audioFormat, numBytesToWrite);

        // TODO: Think about adding some kind of bulk-inserter?
        ret.assign(data, numBytesToWrite);

        ok = m_captureClient->ReleaseBuffer(numFramesToRead);
        if(ok < 0)
        {
            // Seriously? It failed here? How?
            //stop();
            bool breakPoint = false;
        }

        return ret;
    }

    bool  AudioCaptureDeviceImpl::readFromBuffer(AudioStream& out, TaskCallback* callback)
    {
        if(!isStarted())
        {
            start();
            if(!isStarted())
                return false;
        }

        if(!m_captureClient)
            return false;

        bool continueReading = true;
        const size_t maxPacketsFallback = 100000;
        size_t packetsRead = 0;
        while(continueReading)
        {
            // See if there's any data we can get
            //d::this_thread::sleep_for(std::chrono::microseconds(m_referenceTime));
            unsigned int numFramesToRead = 0;
            int ok =  0;
            ok = m_captureClient->GetNextPacketSize(&numFramesToRead);
            //int ok = m_captureClient->GetNextPacketSize(&numFramesToRead);
            if(ok < 0)
            {
                // Unable to get the next packet size... something went wrong
                return false;
            }

            if(numFramesToRead == 0)
            {
                if(callback)
                    continueReading = !callback->isTaskStopped();
                continue;
            }

            AudioByte* data = nullptr;      
            numFramesToRead = 0;
            unsigned long returnFlags = 0;
            ok = m_captureClient->GetBuffer(&data, &numFramesToRead, &returnFlags, 0, 0);
            if(ok < 0 || data == nullptr)
            {
                // Something bad happened when we tried to grab the buffer
                // Releasing the buffer here may or may not be bad...
                //stop();
                m_captureClient->ReleaseBuffer(numFramesToRead);
                return false;
            }

            if(numFramesToRead == 0)
            {
                if(callback)
                    continueReading = !callback->isTaskStopped();
                //m_captureClient->ReleaseBuffer(numFramesToRead);
                continue;
            }

            // TODO: Some logic / error handling based off of flag return values. Skipping that for now.
            const size_t bytesToCopy = numFramesToRead * m_audioFormat.bitsPerBlock;
            AudioPacket ret(m_audioFormat, bytesToCopy);

            ret.assign(data, bytesToCopy);

            // TODO: Think about adding some kind of bulk-inserter?
            //for(size_t i = 0; i < numFramesToRead * m_audioFormat.bitsPerBlock; ++i)
            //    ret[i] = data[i];

            if(out.getAudioFormat().channels != m_audioFormat.channels)
                out.setAudioFormat(m_audioFormat);

            out.push(std::move(ret));

            ok = m_captureClient->ReleaseBuffer(numFramesToRead);
            if(ok < 0)
            {
                // Seriously? It failed here? How?
                bool breakPoint = false;
            }

            continueReading = (callback ? !callback->isTaskStopped() : ++packetsRead < maxPacketsFallback);
        }

        return true;
    }

#endif

}
