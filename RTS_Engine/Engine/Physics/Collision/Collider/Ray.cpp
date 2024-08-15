#include <BennettPCH.h>
#include "Ray.h"

namespace Bennett
{
	Ray::Ray(const glm::vec3& point, const glm::vec3& direction)
	{
		m_Start = point;
		m_Direction = glm::normalize(direction);
	}

	Ray::~Ray()
	{
		m_Start = glm::vec3(0.0f);
		m_Direction = glm::vec3(0.0f);
	}

	const glm::vec3& Ray::GetStart() const
	{
		return m_Start;
	}

	const glm::vec3& Ray::GetDirection() const
	{
		return m_Direction;
	}
}