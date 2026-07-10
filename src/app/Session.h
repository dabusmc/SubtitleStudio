#pragma once

#include <filesystem>

#include "PlaybackState.h"
#include "subtitle/SubtitleTrack.h"

namespace SubtitleStudio
{
	struct Session
	{
		std::filesystem::path VideoPath;
		std::filesystem::path SubtitlePath;

		SubtitleTrack Track;
		PlaybackState Playback;
	};
}