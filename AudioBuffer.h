
#pragma once

#include <AudioDX/AudioFormat.h>

#include <memory>

namespace AudioDX
{

    // TODO: Handle endianess
    // TODO: Think about adding some kind of framing information?
    // TODO: Think about adding some kind of bulk inserter?
    class AudioBuffer
    {
    public:
        
        AudioBuffer(size_t size = 0);
        AudioBuffer(const AudioFormat& format, size_t size = 0);
        AudioBuffer(const AudioBuffer& copy);
        AudioBuffer(AudioBuffer&& move);
        virtual ~AudioBuffer();
  
        AudioByte&          operator[](size_t index);
        AudioByte           at(size_t index) const;
        AudioFormat         getAudioFormat() const;
        void                setAudioFormat(const AudioFormat&);

        size_t              size() const;
        bool                isValid() const;

    private:
        // Don't use this, AudioBuffers should only be copyable on creation
        // Copies are expensive since we're dealing almost exclusively with the heap
        AudioBuffer&        operator=(const AudioBuffer& copy);

        // I hope you know what you're doing here...
        const AudioByte*    data() const;

        size_t                          m_size;        
        std::unique_ptr<AudioByte[]>    m_memory;
        AudioFormat                     m_format;

        // Our impls need to copy them, though
        friend class AbstractAudioDeviceImpl;
        friend class AudioCaptureDeviceImpl;
        friend class AudioPlaybackDeviceImpl;
    };

    size_t  determineBufferSize(const AudioBuffer& in, const AudioFormat& outFormat);

    const AudioBuffer BAD_BUFFER;

}