#pragma once

#include <filesystem>

#include "subtitle/SubtitleTrack.h"

namespace SubtitleStudio
{
	struct Session
	{
		std::filesystem::path VideoPath;
		std::filesystem::path SubtitlePath;

		SubtitleTrack Track;
	};
}