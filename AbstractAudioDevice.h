
#pragma once

#include <memory>

namespace AudioDX
{

	class AbstractAudioDeviceImpl;
	struct AudioFormat;

	// Created by AudioDeviceManager. Don't try to yourself, kids
	class AbstractAudioDevice
	{
	public:

		virtual ~AbstractAudioDevice();
		
		virtual bool initialize() = 0;

		virtual bool start();
		virtual bool stop();

		virtual bool isCaptureDevice() const;
		virtual bool isPlaybackDevice() const;
		virtual bool isValid() const;	

		virtual AudioFormat getAudioFormat() const;	

	protected:
		AbstractAudioDevice();		

		std::shared_ptr<AbstractAudioDeviceImpl> impl;

		friend class AudioDeviceManager;
		friend class AudioDeviceManagerImpl;
	};

}