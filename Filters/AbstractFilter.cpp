
#include <AudioDX/AudioPacket.h>
#include <AudioDX/Filters/AbstractFilter.h>

#pragma warning(push)
#pragma warning(disable : 4244) // Get rid of the pesky "double to size_t" conversion warning

namespace AudioDX
{

    bool AbstractFilter::transformPacket(const AudioPacket& in, AudioPacket& out) const
    {
        // We were given empty buffers :(
        if(in.size() == 0 || out.size() == 0)
            return false;

        const double ratio = double(in.getAudioFormat().samplesPerSecond) / double(out.getAudioFormat().samplesPerSecond);

        for(size_t i = 0; i < out.size(); ++i)
            out[i] = in.at(size_t(double(i) * ratio));

        return true;
    }

    std::string AbstractFilter::name() const
    {
        //static const std::string name("wallstop's Fast and Bad Filter.");
        return std::string("wallstop's Fast and Bad Filter.");
    }

}

#pragma warning(pop)