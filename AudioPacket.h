
#pragma once

#include <AudioDX/AudioFormat.h>

#include <memory>

namespace AudioDX
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // Samples are NOT responsible for their own memory (not yet, at least)
    class Sample
    {
    public:
        Sample();
        Sample(const AudioFormat& format, AudioByte* memory);
        Sample(const Sample& copy);
        Sample(Sample&& move);
        ~Sample();

        Sample& operator=(const Sample& copy);        

    private:

        Sample& operator=(Sample&& move);

        AudioFormat     m_audioFormat;
        AudioByte*      m_memory;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // TODO: Handle endianess
    // TODO: Think about adding some kind of framing information?
    // TODO: Remove format information from individual packets
    class AudioPacket
    {
    public:


        #ifndef DEFAULT_PACKET_SIZE
            #define DEFAULT_PACKET_SIZE 0
        #endif
        
        AudioPacket(size_t size = DEFAULT_PACKET_SIZE);
        AudioPacket(const AudioFormat& format, size_t size = DEFAULT_PACKET_SIZE);
        AudioPacket(const AudioPacket& copy);
        AudioPacket(AudioPacket&& move);
        virtual ~AudioPacket();
  
        Sample             operator[](size_t index);
        AudioPacket&        operator=(AudioPacket&& move);
        // Don't use this, AudioPackets should only be copyable on creation
        // Copies are expensive since we're dealing almost exclusively with the heap
        AudioPacket&        operator=(const AudioPacket& copy);
        const Sample       at(size_t index) const;
        AudioFormat         getAudioFormat() const;
        void                setAudioFormat(const AudioFormat&);

        size_t              byteSize() const;
        size_t              sampleSize() const;
        bool                isValid() const;

        // Hope you know what you're doing
        AudioByte*          data();
        const AudioByte*    data() const;
        void                assign(const void* data, size_t numBytes, size_t offSet = 0);
        void                assign(std::unique_ptr<AudioByte[]>&& data);

    private:
        size_t                          m_size;    
        size_t                          m_maxSize;
        std::unique_ptr<AudioByte[]>    m_memory;
        AudioFormat                     m_format;

    };

    size_t  determineBufferSize(const AudioPacket& in, const AudioFormat& outFormat);

    static const AudioPacket BAD_BUFFER;

}