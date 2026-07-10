#include "Timecode.h"

#include <stdexcept>

namespace SubtitleStudio
{
	namespace Timecode
	{
        constexpr int HoursIndex = 0;
        constexpr int MinutesIndex = 3;
        constexpr int SecondsIndex = 6;
        constexpr int MillisecondsIndex = 9;

        std::chrono::milliseconds FromSRT(const QString& timeString)
        {
            if (timeString.length() != 12)
            {
                throw std::runtime_error("Invalid SRT timestamp length");
            }

            if (timeString[2] != ':' || timeString[5] != ':' || timeString[8] != ',')
            {
                throw std::runtime_error("Invalid SRT timestamp format");
            }

            bool ok;

            int h = timeString.mid(HoursIndex, 2).toInt(&ok);
            if (!ok)
            {
                throw std::runtime_error("Invalid hours");
            }

            int m = timeString.mid(MinutesIndex, 2).toInt(&ok);
            if (!ok)
            {
                throw std::runtime_error("Invalid minutes");
            }

            int s = timeString.mid(SecondsIndex, 2).toInt(&ok);
            if (!ok)
            {
                throw std::runtime_error("Invalid seconds");
            }

            int ms = timeString.mid(MillisecondsIndex, 3).toInt(&ok);
            if (!ok)
            {
                throw std::runtime_error("Invalid milliseconds");
            }

            return std::chrono::hours(h)
                + std::chrono::minutes(m)
                + std::chrono::seconds(s)
                + std::chrono::milliseconds(ms);
        }

		QString ToSRT(std::chrono::milliseconds time)
		{
            auto totalMilliseconds = time.count();

            auto hours = totalMilliseconds / 3600000;
            totalMilliseconds %= 3600000;

            auto minutes = totalMilliseconds / 60000;
            totalMilliseconds %= 60000;

            auto seconds = totalMilliseconds / 1000;
            auto milliseconds = totalMilliseconds % 1000;

            return QString::asprintf("%02lld:%02lld:%02lld,%03lld",
                hours,
                minutes,
                seconds,
                milliseconds);
		}

        QString FormatTimeline(std::chrono::milliseconds time)
        {
            auto totalSeconds = duration_cast<std::chrono::seconds>(time).count();

            auto hours = totalSeconds / 3600;
            totalSeconds %= 3600;

            auto minutes = totalSeconds / 60;
            auto seconds = totalSeconds % 60;

            if (hours == 0)
            {
                return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
            }

            return QString("%1:%2:%3")
                .arg(hours, 2, 10, QChar('0'))
                .arg(minutes, 2, 10, QChar('0'))
                .arg(seconds, 2, 10, QChar('0'));
        }
	}
}