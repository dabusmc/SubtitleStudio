#pragma once

#include "subtitle/Subtitle.h"
#include "subtitle/SubtitleSelection.h"

#include <QPoint>

namespace SubtitleStudio
{
    enum class DragMode
    {
        None,
        Move
    };

    struct SubtitleDragState
    {
        DragMode Mode = DragMode::None;
        QPoint MouseStart;
        SubtitleSelection Selection;
        Subtitle OriginalSubtitle;
        int TargetTrack = -1;
    };
}