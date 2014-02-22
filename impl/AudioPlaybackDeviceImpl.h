
#pragma once

#include <AudioDX/impl/AbstractAudioDeviceImpl.h>

namespace AudioDX
{

#ifdef WIN32

	class AudioPlaybackDeviceImpl : public AbstractAudioDeviceImpl
	{
	public:
		
		AudioPlaybackDeviceImpl(IMMDevice* mmDevice = nullptr,
			IAudioRenderClient* playbackClient = nullptr);
		virtual ~AudioPlaybackDeviceImpl();

		virtual bool initialize(TaskableCallback* callback = nullptr);

	protected:
		IAudioRenderClient*	m_playbackClient;

	};

#endif

}