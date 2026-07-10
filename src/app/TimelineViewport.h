#pragma once

#include <chrono>

namespace SubtitleStudio
{
	struct TimelineViewport
	{
		std::chrono::milliseconds Start = std::chrono::milliseconds(0);
		std::chrono::milliseconds Duration = std::chrono::seconds(60);
	};
}