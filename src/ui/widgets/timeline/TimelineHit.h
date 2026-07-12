#pragma once

#include "subtitle/SubtitleSelection.h"

namespace SubtitleStudio
{
    enum class TimelineHitType
    {
        None,
        Body,
        LeftEdge,
        RightEdge
    };

    struct TimelineHit
    {
        TimelineHitType Type = TimelineHitType::None;

        SubtitleSelection Selection;
    };
}