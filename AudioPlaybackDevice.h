
#pragma once

#include <AudioDX/AbstractAudioDevice.h>

namespace AudioDX
{

	class AudioPlaybackDeviceImpl;

	class AudioPlaybackDevice : public AbstractAudioDevice
	{
	public:

		AudioPlaybackDevice();
		virtual ~AudioPlaybackDevice();

		virtual bool initialize();

		virtual bool isPlaybackDevice() const;
	};
}