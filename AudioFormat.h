
#pragma once

namespace AudioDX
{

    typedef unsigned char AudioByte;

    struct AudioFormat
    {
        unsigned short  channels;
        unsigned int    samplesPerSecond;
        unsigned short  bitsPerBlock;
        unsigned int    bitsPerSample;

        bool operator==(const AudioFormat& other) const;
        bool operator!=(const AudioFormat& other) const;

    };

}