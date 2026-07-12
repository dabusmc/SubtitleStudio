#pragma once

#include <chrono>
#include <functional>

#include "SubtitleDragState.h"
#include "app/Session.h"
#include "subtitle/SubtitleSelection.h"

#include <QPoint>

namespace SubtitleStudio
{
    class SubtitleEditor
    {
    public:
        SubtitleEditor(Session& session);

        const SubtitleDragState& DragState() const { return m_DragState; }
        const SubtitleSelection& Selection() const { return m_Selection; }

        void SetOnEdited(std::function<void()> onEdited) { m_OnEdited = onEdited; }

        void BeginDrag(const SubtitleSelection& selection, QPoint mouseStart);
        void DragBy(std::chrono::milliseconds delta, int targetTrack);
        void EndDrag();

    private:
        SubtitleTrack& CreateTrack();

    private:
        Session& m_Session;
        SubtitleDragState m_DragState;
        SubtitleSelection m_Selection;
        std::function<void()> m_OnEdited;
    };
}