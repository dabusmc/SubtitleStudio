#pragma once

#include <vector>

#include "Subtitle.h"

namespace SubtitleStudio
{
	struct SubtitleTrack
	{
		std::vector<Subtitle> Subtitles;
	};
}

std::ostream& operator<<(std::ostream& os, const SubtitleStudio::SubtitleTrack& track);