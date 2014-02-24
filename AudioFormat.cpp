
#include <AudioDX/AudioFormat.h>

namespace AudioDX
{

    bool AudioFormat::operator==(const AudioFormat& other) const
    {
        return channels         == other.channels 
            && samplesPerSecond == other.samplesPerSecond
            && bitsPerBlock     == other.bitsPerBlock
            && bitsPerSample    == other.bitsPerSample;
    }

    bool AudioFormat::operator!=(const AudioFormat& other) const
    {
        return !(*this == other);
    }

}