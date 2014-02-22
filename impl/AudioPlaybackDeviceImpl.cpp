
#include <AudioDX/impl/AudioPlaybackDeviceImpl.h>

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
		}

		m_audioFormat.channels			= waveFormat->nChannels;
		m_audioFormat.samplesPerSecond	= waveFormat->nSamplesPerSec;
		m_audioFormat.bitsPerBlock		= waveFormat->nBlockAlign;
		m_audioFormat.bitsPerSample		= waveFormat->wBitsPerSample;

        // Try to intialize our audio client
		ok = m_client->Initialize(AUDCLNT_SHAREMODE_SHARED, 0,
			REFERENCE_TIMES_PER_SECOND, 0, waveFormat, 0);
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

#endif

}