
#include <AudioDX/AudioPacket.h>

#include <assert.h>
#include <stdexcept>
#include <memory>

namespace AudioDX
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // Sample impl

    Sample::Sample() : m_memory(nullptr)
    {
    }

    Sample::Sample(const AudioFormat& format, AudioByte* memory) : m_audioFormat(format), m_memory(memory)
    {
    }

    Sample::Sample(const Sample& copy)
    {
        m_memory = copy.m_memory;
        m_audioFormat = copy.m_audioFormat;
    }

    Sample::Sample(Sample&& move)
    {
        m_memory = move.m_memory;
        m_audioFormat = std::move(move.m_audioFormat);
    }

    Sample& Sample::operator=(const Sample& copy)
    {
        // This is a bug - 2 channels, 2 byte will auto conver to 4 channels, 1 byte... (and similar)
        // TODO: better copying methodology
        if(copy.m_audioFormat.bitsPerBlock == m_audioFormat.bitsPerBlock)
        {
            std::memcpy(m_memory, copy.m_memory, copy.m_audioFormat.bitsPerBlock);
            return *this;
        }

        throw std::bad_alloc();
    }

    Sample::~Sample()
    {
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // AudioPacket impl

    AudioPacket::AudioPacket(size_t size) : m_size(size), m_maxSize(size), m_memory(nullptr)
    {
        if(m_size > 0)
         m_memory = std::unique_ptr<AudioByte[]>(new AudioByte[size]);
    }

    AudioPacket::AudioPacket(const AudioFormat& format, size_t size) 
        : m_size(size), m_maxSize(size), m_memory(nullptr), m_format(format)
    {
        if(m_size > 0)
            m_memory = std::unique_ptr<AudioByte[]>(new AudioByte[size]);
    }

    AudioPacket::AudioPacket(const AudioPacket& copy) 
        : m_size(copy.m_size), m_maxSize(copy.m_size), m_memory(nullptr), m_format(copy.m_format)
    {
        assert(copy.m_size > 0);
        if(copy.m_size > 0)
        {
            m_memory = std::unique_ptr<AudioByte[]>(new AudioByte[copy.m_size]);
            assign(copy.m_memory.get(), copy.m_size); 
        }
        assert(m_memory.get() != nullptr);
    }

    AudioPacket::AudioPacket(AudioPacket&& move) : m_size(0), m_memory(nullptr)
    {
        // rely on our operator=(AudioPacket&&)
        *this = move;
    }

    AudioPacket::~AudioPacket()
    {
    }

    Sample AudioPacket::operator[](size_t index)
    {
        assert(m_memory.get() != nullptr);
        const size_t realIndex = index * m_format.bitsPerBlock;
        if(realIndex >= m_size)
        {
            throw std::out_of_range("");
        }

        return Sample(m_format, &m_memory[realIndex]);
    }

    const Sample AudioPacket::at(size_t index) const
    {
        assert(m_memory.get() != nullptr);
        const size_t realIndex = index * m_format.bitsPerBlock;
        if(realIndex >= m_size)
        {
            throw std::out_of_range("");
        }

        return Sample(m_format, &m_memory[realIndex]);
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
        return (m_memory.get() != nullptr && m_format != AudioFormat());
    }

    AudioPacket& AudioPacket::operator=(const AudioPacket& copy)
    {
        assert(copy.m_size > 0);
        if(copy.m_size >= m_maxSize)
        {
            m_memory = std::unique_ptr<AudioByte[]>(new AudioByte[copy.m_size]);
            m_size = copy.m_size;
        }
        m_format = copy.m_format;
        assign(copy.m_memory.get(), copy.m_size);

        return *this;
    }

    AudioPacket& AudioPacket::operator=(AudioPacket&& move)
    {
        m_size = move.m_size;
        m_maxSize = move.m_maxSize;
        m_format = move.m_format;
        m_memory = std::move(move.m_memory);
        move.m_memory = nullptr;
        move.m_size = 0;
        move.m_maxSize = 0;
        move.m_format = AudioFormat();
        return *this;
    }

    AudioByte* AudioPacket::data()
    {
        return m_memory.get();
    }

    const AudioByte* AudioPacket::data() const
    {
        return const_cast<const AudioByte*>(m_memory.get());
    }

    // risky
    void AudioPacket::assign(const void* data, size_t numBytes, size_t offSet)
    {
        assert(m_memory.get());
        assert(m_maxSize >= numBytes + offSet);
        std::memcpy((m_memory.get() + offSet), data, numBytes);
    }

    void AudioPacket::assign(std::unique_ptr<AudioByte[]>&& data)
    {
        m_memory = std::move(data);
    }

    size_t AudioPacket::byteSize() const
    {
        return m_size;
    }

    size_t AudioPacket::sampleSize() const
    {
        return (m_size / m_format.bitsPerBlock);
    }

    size_t determineBufferSize(const AudioPacket& in, const AudioFormat& outFormat)
    {
        const unsigned int numInSamples = in.getAudioFormat().samplesPerSecond;
        const unsigned int numOutSamples = outFormat.samplesPerSecond;

        const unsigned short bitsPerInBlock = in.getAudioFormat().bitsPerBlock;
        const unsigned short bitsPerOutBlock = outFormat.bitsPerBlock;

        const double ratio = (double(numOutSamples) / double(numInSamples)) * (double(bitsPerOutBlock) / double(bitsPerInBlock));

        return size_t(ratio * in.byteSize());
    }

}
