
#include <AudioDX/AudioBuffer.h>

#include <stdexcept>

namespace AudioDX
{
    AudioBuffer::AudioBuffer(size_t size) : m_size(size), m_memory(new AudioByte[size])
    {
    }

    AudioBuffer::AudioBuffer(const AudioFormat& format, size_t size) 
        : m_size(size), m_memory(new AudioByte[size]), m_format(format)
    {
    }

    AudioBuffer::AudioBuffer(const AudioBuffer& copy) 
        : m_size(copy.m_size), m_memory(new AudioByte[copy.m_size]), m_format(copy.m_format)
    {
        for(size_t i = 0; i < m_size; ++i)
            m_memory[i] = copy.m_memory[i];
    }

    AudioBuffer::AudioBuffer(AudioBuffer&& move) : m_size(move.m_size), m_memory(std::move(move.m_memory)), m_format(move.m_format)
    {
        move.m_memory = nullptr;
    }

    AudioBuffer::~AudioBuffer()
    {
    }

    AudioByte& AudioBuffer::operator[](size_t index)
    {
        if(index >= m_size)
        {
            throw std::out_of_range("");
        }

        return m_memory[index];
    }

    AudioByte AudioBuffer::at(size_t index) const
    {
        if(index >= m_size)
            return AudioByte();
        return m_memory[index];    
    }

    AudioFormat AudioBuffer::getAudioFormat() const
    {
        return m_format;
    }

    void AudioBuffer::setAudioFormat(const AudioFormat& format)
    {
        m_format = format;
    }

    bool AudioBuffer::isValid() const
    {
        return (m_memory != nullptr && m_format != AudioFormat());
    }

    AudioBuffer& AudioBuffer::operator=(const AudioBuffer& copy)
    {
        m_size = copy.m_size;
        m_format = copy.m_format;
        auto ptr = m_memory.release();
        if(ptr)
            delete ptr;
        m_memory = std::unique_ptr<AudioByte[]>(new AudioByte[m_size]);
        for(size_t i = 0; i < m_size; ++i)
            m_memory[i] = copy.at(i);

        return *this;
    }

    AudioBuffer& AudioBuffer::operator=(AudioBuffer&& move)
    {
        m_size = move.m_size;
        m_format = move.m_format;
        auto ptr = m_memory.release();
        if(ptr)
            delete ptr;
        m_memory = std::move(move.m_memory);
        move.m_memory = nullptr;
        return *this;
    }

    const AudioByte* AudioBuffer::data() const
    {
        return m_memory.get();
    }

    size_t AudioBuffer::size() const
    {
        return m_size;
    }

    size_t determineBufferSize(const AudioBuffer& in, const AudioFormat& outFormat)
    {
        const unsigned int numInSamples     = in.getAudioFormat().samplesPerSecond;
        const unsigned int numOutSamples    = outFormat.samplesPerSecond;

        const double ratio = double(numInSamples) / double(numOutSamples);

        return size_t(ratio * in.size());
    }

}
