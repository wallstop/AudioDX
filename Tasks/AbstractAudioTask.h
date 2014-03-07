
#pragma once

#include <AudioDX/AbstractAudioDevice.h>
#include <AudioDX/Tasks/TaskCallback.h>

namespace AudioDX
{

	class AbstractAudioTask
	{
	public:

		AbstractAudioTask() : m_device(nullptr) 
		{
		}

		AbstractAudioTask(AbstractAudioDevice* device) : m_device(device)
		{
		}

		virtual ~AbstractAudioTask()
		{
		}

		virtual void run(TaskCallback *callback = nullptr) = 0;

	protected:

		std::shared_ptr<AbstractAudioDevice> m_device;
	};

}