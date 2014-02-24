
#pragma once

namespace AudioDX
{

    typedef unsigned char AudioByte;

    struct AudioFormat
    {
        unsigned int channels;
        unsigned int samplesPerSecond;
        unsigned int bitsPerBlock;
        unsigned int bitsPerSample;

        bool operator==(const AudioFormat& other) const;
        bool operator!=(const AudioFormat& other) const;

    };

}