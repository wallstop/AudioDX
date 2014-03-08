
#pragma once

#include <memory>
#include <string>

namespace AudioDX
{

    class   AbstractAudioDeviceImpl;
    struct  AudioFormat;
    struct  AbstractFilter;
    class   AudioPacket;
    class   AudioStream;
    class   TaskCallback;

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

        virtual AudioFormat     getAudioFormat() const;	
        virtual AudioPacket     readFromBuffer() = 0;   
        //virtual AudioStream     readFromBuffer(TaskCallback* callback) = 0;
        virtual bool            readFromBuffer(AudioStream& out, TaskCallback* callback) = 0;
        virtual bool            writeToBuffer(const AudioPacket& in, const AbstractFilter& filter) = 0;
        virtual bool            writeToBuffer(AudioStream& in, const AbstractFilter& filter, TaskCallback* callback = nullptr) = 0;

        virtual std::string     id() const;

    protected:
        AbstractAudioDevice();		
        AbstractAudioDevice(const AbstractAudioDevice&);
        AbstractAudioDevice(AbstractAudioDevice&&);

        std::shared_ptr<AbstractAudioDeviceImpl> impl;

        friend class AudioDeviceManager;
        friend class AudioDeviceManagerImpl;
    };

}