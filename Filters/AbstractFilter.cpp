
#include <AudioDX/AudioPacket.h>
#include <AudioDX/Filters/AbstractFilter.h>

#pragma warning(push)
#pragma warning(disable : 4244) // Get rid of the pesky "double to size_t" conversion warning

namespace AudioDX
{

    bool AbstractFilter::transformPacket(const AudioPacket& in, AudioPacket& out) const
    {
        // We were given empty buffers :(
        if(in.byteSize() == 0 || out.byteSize() == 0)
            return false;

        const double ratio = double(in.getAudioFormat().samplesPerSecond) / double(out.getAudioFormat().samplesPerSecond);
        if(ratio == 1.)
        {
            out.assign(in.data(), in.byteSize());
            return true;
        }

        // TODO: bit conversions
        const unsigned char inBitsPerBlock = in.getAudioFormat().bitsPerBlock;
        const unsigned char outBitsPerBlock = out.getAudioFormat().bitsPerBlock;
        const size_t maxSamples = out.sampleSize();

        for(size_t i = 0; i < maxSamples; ++i)
        {
            out[i] = in.at(size_t(double(i) * ratio));
        }

        return true;
    }

    std::string AbstractFilter::name() const
    {
        //static const std::string name("wallstop's Fast and Bad Filter.");
        return std::string("wallstop's Fast and Bad Filter.");
    }

}

#pragma warning(pop)