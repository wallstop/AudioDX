
#pragma once

#include <AudioDX/AudioFormat.h>

#include <memory>

namespace AudioDX
{

    // TODO: Handle endianess
    // TODO: Think about adding some kind of framing information?
    // TODO: Think about adding some kind of bulk inserter?
    class AudioPacket
    {
    public:

        #ifndef DEFAULT_PACKET_SIZE
            #define DEFAULT_PACKET_SIZE 10
        #endif
        
        AudioPacket(size_t size = DEFAULT_PACKET_SIZE);
        AudioPacket(const AudioFormat& format, size_t size = DEFAULT_PACKET_SIZE);
        AudioPacket(const AudioPacket& copy);
        AudioPacket(AudioPacket&& move);
        virtual ~AudioPacket();
  
        AudioByte&      operator[](size_t index);
        AudioPacket&    operator=(AudioPacket&& move);
        // Don't use this, AudioPackets should only be copyable on creation
        // Copies are expensive since we're dealing almost exclusively with the heap
        AudioPacket&    operator=(const AudioPacket& copy);
        AudioByte       at(size_t index) const;
        AudioFormat     getAudioFormat() const;
        void            setAudioFormat(const AudioFormat&);

        size_t          size() const;
        bool            isValid() const;

        // Hope you know what you're doing
        AudioByte*      data();

    private:
        size_t          m_size;        
        AudioByte*      m_memory;
        AudioFormat     m_format;

    };

    size_t  determineBufferSize(const AudioPacket& in, const AudioFormat& outFormat);

    static const AudioPacket BAD_BUFFER;

}