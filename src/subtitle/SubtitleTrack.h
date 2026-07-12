#pragma once

#include <string>
#include <vector>

#include "Subtitle.h"

namespace SubtitleStudio
{
	struct SubtitleTrack
	{
		std::string Name;
		bool Visible = true;
		bool Locked = false;

		std::vector<Subtitle> Subtitles;
	};
}

std::ostream& operator<<(std::ostream& os, const SubtitleStudio::SubtitleTrack& track);