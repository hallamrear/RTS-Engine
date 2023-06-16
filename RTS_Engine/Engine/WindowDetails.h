#pragma once

namespace Bennett
{
	struct WindowDetails
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowDetails(
			const std::string& title = "Bennett Engine",
			unsigned int width = WINDOW_WIDTH,
			unsigned int height = WINDOW_HEIGHT)
			: Title(title), Width(width), Height(height)
		{

		}
	};
}