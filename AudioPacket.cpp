
#include <AudioDX/AudioPacket.h>

#include <assert.h>
#include <stdexcept>

namespace AudioDX
{
    AudioPacket::AudioPacket(size_t size) : m_size(size), m_memory(nullptr)
    {
        assert(m_size > 0);
        m_memory = new AudioByte[size];
    }

    AudioPacket::AudioPacket(const AudioFormat& format, size_t size) 
        : m_size(size), m_memory(nullptr), m_format(format)
    {
        assert(m_size > 0);
        m_memory = new AudioByte[size];
    }

    AudioPacket::AudioPacket(const AudioPacket& copy) 
        : m_size(copy.m_size), m_memory(nullptr), m_format(copy.m_format)
    {
        assert(copy.m_size > 0);
        if(copy.m_size > 0)
        {
            m_memory = new AudioByte[copy.m_size];
            std::memcpy(m_memory, copy.m_memory, copy.m_size * sizeof(AudioByte));
        }
    }

    AudioPacket::AudioPacket(AudioPacket&& move) : m_size(0), m_memory(nullptr)
    {
        // rely on our operator=(AudioPacket&&)
        *this = move;
    }

    AudioPacket::~AudioPacket()
    {
        delete[] m_memory;
    }

    AudioByte& AudioPacket::operator[](size_t index)
    {
        assert(m_memory);
        if(index >= m_size)
        {
            throw std::out_of_range("");
        }

        return m_memory[index];
    }

    AudioByte AudioPacket::at(size_t index) const
    {
        assert(m_memory);
        if(index >= m_size)
            return AudioByte();
        return m_memory[index];    
    }

    AudioFormat AudioPacket::getAudioFormat() const
    {
        return m_format;
    }

    void AudioPacket::setAudioFormat(const AudioFormat& format)
    {
        m_format = format;
    }

    bool AudioPacket::isValid() const
    {
        return (m_memory != nullptr && m_format != AudioFormat());
    }

    AudioPacket& AudioPacket::operator=(const AudioPacket& copy)
    {
        assert(copy.m_size > 0);
        delete[] m_memory;
        m_memory = new AudioByte[copy.m_size];

        m_size = copy.m_size;
        m_format = copy.m_format;
        
        std::memcpy(m_memory, copy.m_memory, m_size * sizeof(AudioByte));

        return *this;
    }

    AudioPacket& AudioPacket::operator=(AudioPacket&& move)
    {
        m_size = move.m_size;
        m_format = move.m_format;
        m_memory = move.m_memory ? std::move(move.m_memory) : nullptr;
        move.m_memory = nullptr;
        move.m_size = 0;
        return *this;
    }

    // risky
    AudioByte* AudioPacket::data()
    {
        return m_memory;
    }

    size_t AudioPacket::size() const
    {
        return m_size;
    }

    size_t determineBufferSize(const AudioPacket& in, const AudioFormat& outFormat)
    {
        const unsigned int numInSamples     = in.getAudioFormat().samplesPerSecond;
        const unsigned int numOutSamples    = outFormat.samplesPerSecond;

        const double ratio = double(numOutSamples) / double(numInSamples);

        return size_t(ratio * in.size());
    }

}
