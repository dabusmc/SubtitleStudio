#pragma once

#include <filesystem>

#include "SubtitleTrack.h"

namespace SubtitleStudio
{
	namespace SRT
	{
		SubtitleTrack Load(const std::filesystem::path& path);
		void Save(const SubtitleTrack& track, const std::filesystem::path& path);
	}
}