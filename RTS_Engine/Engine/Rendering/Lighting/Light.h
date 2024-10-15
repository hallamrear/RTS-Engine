#pragma once

namespace Bennett
{
	struct BENNETT_ENGINE Light
	{
		glm::vec4 RGBI;
		glm::vec4 Position;

		Light()
		{
			Position = glm::vec4(0.0f);
			RGBI = glm::vec4(1.0f);
		}
	};
}

