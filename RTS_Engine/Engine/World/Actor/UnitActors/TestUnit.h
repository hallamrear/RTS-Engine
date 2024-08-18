#pragma once
#include "World/Actor/BActor.h"

namespace Bennett
{
    class BENNETT_ENGINE TestUnit :
        public BActor
    {
    public:
        TestUnit(const std::string& name, const Transform& transform);
        ~TestUnit();

        void Render(const Renderer& renderer);
    };
}