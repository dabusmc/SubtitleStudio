#include "SubtitleTrack.h"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const SubtitleStudio::SubtitleTrack& track)
{
    os << "Track\n";

    for (const auto& subtitle : track.Subtitles)
    {
        os << "\t" << subtitle << '\n';
    }

    return os;
}