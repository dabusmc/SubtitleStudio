#pragma once

#include <chrono>

namespace SubtitleStudio
{
    struct PlaybackState
    {
        std::chrono::milliseconds Position { 0 };
        std::chrono::milliseconds Duration { 0 };
        bool Playing = false;
        double Speed = 1.0;
    };
}