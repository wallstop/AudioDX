////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
// AudioStream is a single reader, single writer concurrent queue used to stream audio packets
// from an AudioCaptureDevice to an AudioPlaybackDevice
// Author: Eli Pinkerton
// Date: 2/26/14
////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <ConcurrentDX/Containers/ConcurrentStream.h>
#include <AudioDX/AudioFormat.h>
#include <AudioDX/AudioPacket.h>

namespace AudioDX
{

    class AudioStream
    {
    public:
        AudioStream();
        AudioStream(const AudioFormat& format);
        ~AudioStream();

        void        setAudioFormat(const AudioFormat& format);
        AudioFormat getAudioFormat() const;
        bool        isEmpty() const;
        size_t      size() const;
        bool        front(AudioPacket& out) const;
        bool        pop(AudioPacket& out);
        void        push(const AudioPacket& in);

        AudioStream&    operator<<(const AudioPacket& in);
        bool            operator>>(AudioPacket& out);

    private:
        DX::ConcurrentStream<AudioPacket>   m_stream;
        AudioFormat                         m_format;
    };

}
