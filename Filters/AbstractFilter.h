
#pragma once

#include <string>

namespace AudioDX
{

    class AudioPacket;

    // TODO: Determine a best way to auto-register filters. Template metaprogramming?
    // Need to figure out how to properly import / export these as dlls. This should really be pure virtual
    struct AbstractFilter
    {
    public:
        AbstractFilter(){}
        virtual ~AbstractFilter(){}

        virtual bool            transformPacket(const AudioPacket& in, AudioPacket& out) const;
        virtual std::string     name() const;
    };

}
