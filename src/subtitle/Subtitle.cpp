#include "Subtitle.h"

#include "util/TimeCode.h"

std::ostream& operator<<(std::ostream& os, const SubtitleStudio::Subtitle& subtitle)
{
    os << SubtitleStudio::Timecode::ToSRT(subtitle.Start).toStdString()
        << " - "
        << SubtitleStudio::Timecode::ToSRT(subtitle.End).toStdString()
        << ": "
        << subtitle.Text.toStdString();

    return os;
}