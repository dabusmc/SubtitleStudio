#pragma once

#include <chrono>

#include <QString>

namespace SubtitleStudio
{
	namespace Timecode
	{
		std::chrono::milliseconds FromSRT(const QString& timeString);
		QString ToSRT(std::chrono::milliseconds time);
	}
}