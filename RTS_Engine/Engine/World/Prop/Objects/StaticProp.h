#pragma once
#include <World/Prop/BProp.h>

namespace Bennett
{
    class StaticProp :
        public BProp
    {
    public:
        StaticProp(const std::string& name, const Transform& transform);
        ~StaticProp();
    };
}
