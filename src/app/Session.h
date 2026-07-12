#pragma once

#include <filesystem>
#include <vector>

#include "PlaybackState.h"
#include "TimelineViewport.h"
#include "subtitle/SubtitleTrack.h"

namespace SubtitleStudio
{
	struct Session
	{
		std::filesystem::path VideoPath;
		std::filesystem::path SubtitlePath;

		std::vector<SubtitleTrack> Tracks;
		int ActiveTrack = -1;
		
		PlaybackState Playback;

		TimelineViewport Viewport;
	};
}