#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Subtitle.h"

namespace SubtitleStudio
{
	struct SubtitleTrack
	{
		std::string Name;
		bool Visible = true;
		bool Locked = false;

		std::vector<std::unique_ptr<Subtitle>> Subtitles;
	};
}

std::ostream& operator<<(std::ostream& os, const SubtitleStudio::SubtitleTrack& track);