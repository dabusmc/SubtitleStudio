#pragma once

namespace SubtitleStudio
{
    struct Subtitle;

    struct SubtitleSelection
    {
        int TrackIndex = -1;
        Subtitle* Subtitle = nullptr;
    };
}