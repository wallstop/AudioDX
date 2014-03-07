////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// AudioStream is a single reader, single writer concurrent queue used to stream audio packets
// from an AudioCaptureDevice to an AudioPlaybackDevice
// Author: Eli Pinkerton
// Date: 3/1/14
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "AudioStream.h"

namespace AudioDX
{

    ////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // impl

    AudioStream::AudioStream()
    {
    }

    AudioStream::AudioStream(const AudioFormat& format) : m_format(format)
    {
    }

    AudioStream::~AudioStream()
    {
    }

    void AudioStream::setAudioFormat(const AudioFormat& format)
    {
        m_format = format;
    }

    AudioFormat AudioStream::getAudioFormat() const
    {
        return m_format;
    }

    bool AudioStream::isEmpty() const
    {
        return m_stream.isEmpty();
    }

    size_t AudioStream::size() const
    {
        return m_stream.size();
    }

    bool AudioStream::front(AudioPacket& out) const
    {
        return m_stream.front(out);
    }

    bool AudioStream::pop(AudioPacket& out)
    {
        return m_stream.pop(out);
    }

    void AudioStream::push(const AudioPacket& in)
    {
        m_stream.push(in);
    }

    AudioStream& AudioStream::operator<<(const AudioPacket& in)
    {
        m_stream << in;
        return *this;
    }

    bool AudioStream::operator>>(AudioPacket& out)
    {
        return m_stream >> out;
    }

}

