
#pragma once

namespace AudioDX
{

    struct AudioFormat
    {
        unsigned int channels;
        unsigned int samplesPerSecond;
        unsigned int bitsPerBlock;
        unsigned int bitsPerSample;
    };

}