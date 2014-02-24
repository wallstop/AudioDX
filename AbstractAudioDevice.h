
#pragma once

#include <memory>

namespace AudioDX
{

    class   AbstractAudioDeviceImpl;
    struct  AudioFormat;
    struct  AbstractFilter;
    class   AudioBuffer;

    /*!
        \brief  AbstractAudioDevice is the base class for any system-recognized
                hardware 


    */
    class AbstractAudioDevice
    {
    public:

        virtual ~AbstractAudioDevice() = 0;

        virtual bool initialize() = 0;

        virtual bool start();
        virtual bool stop();

        virtual bool isCaptureDevice() const;
        virtual bool isPlaybackDevice() const;
        virtual bool isValid() const;	

        virtual AudioFormat getAudioFormat() const;	
        virtual AudioBuffer readFromBuffer() = 0;
        virtual bool        writeToBuffer(const AudioBuffer& in, const AbstractFilter& filter) = 0;

    protected:
        AbstractAudioDevice();		

        std::shared_ptr<AbstractAudioDeviceImpl> impl;

        friend class AudioDeviceManager;
        friend class AudioDeviceManagerImpl;
    };

}