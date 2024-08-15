#pragma once

namespace Bennett
{
	class BENNETT_ENGINE Ray
	{
	private:
		glm::vec3 m_Start;
		glm::vec3 m_Direction;

	public:
		Ray(const glm::vec3& point, const glm::vec3& direction);
		~Ray();

		const glm::vec3& GetStart() const;
		const glm::vec3& GetDirection() const;
	};
}