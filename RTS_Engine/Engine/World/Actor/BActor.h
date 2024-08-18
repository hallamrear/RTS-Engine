#pragma once
#include <World/Prop/BProp.h>

namespace Bennett
{
    constexpr glm::vec3 NullTargetPosition = glm::vec3(0.0f, FLT_MAX, 0.0f);

    class BENNETT_ENGINE BActor :
        public BProp
    {
    private:
        bool m_IsSelected;
        bool m_CanBeSelected;
        glm::vec3 m_TargetPosition;

    protected:

        class InputMonitor* m_InputMonitor;
        BActor(const std::string& name, const Transform& transform);

    public:
        virtual ~BActor() = 0;

        const glm::vec3& GetTargetPosition() const;
        void SetTargetPosition(const glm::vec3& position);

        void SetCanBeSelected(const bool state);
        const bool CanBeSelected() const;
        void SetIsSelected(const bool state);
        const bool IsSelected() const;

        virtual void Update(const float& deltaTime);
        virtual void Render(const Renderer& renderer);
    };
}
