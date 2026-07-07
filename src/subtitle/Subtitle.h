#pragma once

#include <chrono>

#include <QString>

namespace SubtitleStudio
{
	struct Subtitle
	{
		std::chrono::milliseconds Start;
		std::chrono::milliseconds End;
		QString Text;
	};

}

std::ostream& operator<<(std::ostream& os, const SubtitleStudio::Subtitle& subtitle);