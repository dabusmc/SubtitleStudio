#include "TimelineWidget.h"

#include <algorithm>

#include "ui/Theme.h"
#include "util/Timecode.h"

#include <QPainter>

namespace SubtitleStudio
{
    namespace
    {
        constexpr int RulerHeight = 20;
        constexpr auto TickSpacing = std::chrono::seconds(5);

        constexpr int TrackTop = Theme::Metrics::TimelineTopPadding + RulerHeight + Theme::Metrics::TimelineTrackSpacing;
    }

    TimelineWidget::TimelineWidget(QWidget* parent)
        : QWidget(parent), m_StudioApp(nullptr)
    {
        setMinimumHeight(150);

        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    void TimelineWidget::SetApplication(Application* app)
    {
        m_StudioApp = app;
        connect(m_StudioApp, &Application::PlaybackPositionChanged, this, [this](std::chrono::milliseconds) {
                update();
            });
        connect(m_StudioApp, &Application::SessionChanged, this, [this]() {
                update();
            });
    }
    
    void TimelineWidget::paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        painter.fillRect(rect(), Theme::Colours::Background);

        if (!m_StudioApp)
            return;

        DrawRuler(painter);
        DrawTracks(painter);
        DrawPlayhead(painter);
        DrawDraggedSubtitle(painter);
    }

    void TimelineWidget::mousePressEvent(QMouseEvent* event)
    {
        if (!m_StudioApp)
            return;

        TimelineHit hit = HitTest(event->pos());
        if (hit.Type == TimelineHitType::Body)
        {
            m_StudioApp->GetSubtitleEditor().BeginDrag(hit.Selection, event->pos());
            return;
        }

        TimelineViewport& viewport = m_StudioApp->GetSession().Viewport;
        auto seekPosition = std::clamp(XToTime(event->pos().x()), viewport.Start, viewport.Start + viewport.Duration);
        m_StudioApp->GetVideoPlayer().Seek(seekPosition);
        update();
    }

    void TimelineWidget::mouseDoubleClickEvent(QMouseEvent* event)
    {
        if (!m_StudioApp)
            return;

        TimelineHit hit = HitTest(event->pos());
        if (hit.Type == TimelineHitType::Body)
        {
            emit SubtitlePropertiesOpen(hit.Selection.Subtitle);
        }
    }

    void TimelineWidget::mouseMoveEvent(QMouseEvent* event)
    {
        if (!(event->buttons() & Qt::LeftButton))
            return;

        auto start = XToTime(m_StudioApp->GetSubtitleEditor().DragState().MouseStart.x());
        auto current = XToTime(event->pos().x());
        auto delta = current - start;

        int targetTrack = YToTrack(event->pos().y());

        m_StudioApp->GetSubtitleEditor().DragBy(delta, targetTrack);
        update();
    }

    void TimelineWidget::mouseReleaseEvent(QMouseEvent* event)
    {
        m_StudioApp->GetSubtitleEditor().EndDrag();
    }

    void TimelineWidget::DrawTracks(QPainter& painter)
    {
        const auto& tracks = m_StudioApp->GetSession().Tracks;

        for (int i = 0; i < static_cast<int>(tracks.size()); ++i)
        {
            DrawTrack(painter, tracks[i], i);
        }

        const auto& drag = m_StudioApp->GetSubtitleEditor().DragState();
        if (IsPreviewTrack(drag.TargetTrack))
        {
            DrawEmptyTrack(painter, drag.TargetTrack);
        }
    }

    void TimelineWidget::DrawTrack(QPainter& painter, const SubtitleTrack& track, int trackIndex)
    {
        int top = TrackTop(trackIndex);

        QRect trackRect(Theme::Metrics::TimelineMargin, top, width() - Theme::Metrics::TimelineMargin * 2, Theme::Metrics::TimelineTrackHeight);
        painter.fillRect(trackRect, Theme::Colours::Track);

        const auto& drag = m_StudioApp->GetSubtitleEditor().DragState();
        if (trackIndex == drag.TargetTrack && drag.Mode == DragMode::Move)
        {
            painter.setPen(QPen(Theme::Colours::AccentSelected,2));
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(trackRect);
        }

        const auto& subtitles = track.Subtitles;

        const auto& selection = m_StudioApp->GetSubtitleEditor().Selection();

        TimelineViewport& viewport = m_StudioApp->GetSession().Viewport;
        for (const auto& subtitle : subtitles)
        {
            if (subtitle->End < viewport.Start)
                continue;

            if (subtitle->Start > viewport.Start + viewport.Duration)
                continue;

            if (drag.Mode == DragMode::Move && selection.Subtitle == subtitle.get())
                continue;

            QRect box = GetSubtitleRect(*subtitle, trackIndex);

            painter.save();
            painter.setClipRect(trackRect);

            painter.setBrush(selection.Subtitle == subtitle.get() ? Theme::Colours::AccentSelected : Theme::Colours::Accent);

            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(box, Theme::Metrics::BorderRadius, Theme::Metrics::BorderRadius);

            painter.setPen(Theme::Colours::Text);
            painter.drawText(box, Qt::AlignCenter, subtitle->Text);

            painter.restore();
        }
    }

    void TimelineWidget::DrawDraggedSubtitle(QPainter& painter)
    {
        const auto& drag = m_StudioApp->GetSubtitleEditor().DragState();

        if (drag.Mode != DragMode::Move)
            return;

        if (!drag.Selection.Subtitle)
            return;

        QRect box = GetSubtitleRect(*drag.Selection.Subtitle, drag.TargetTrack);

        QRect trackRect(Theme::Metrics::TimelineMargin, TrackTop(drag.TargetTrack), width() - Theme::Metrics::TimelineMargin * 2, Theme::Metrics::TimelineTrackHeight);

        painter.save();

        painter.setClipRect(trackRect);

        painter.setBrush(Theme::Colours::AccentSelected);
        painter.setPen(Qt::NoPen);

        painter.drawRoundedRect(box, Theme::Metrics::BorderRadius, Theme::Metrics::BorderRadius);

        painter.setPen(Theme::Colours::Text);

        painter.drawText(box, Qt::AlignCenter, drag.Selection.Subtitle->Text);

        painter.restore();
    }

    void TimelineWidget::DrawEmptyTrack(QPainter& painter, int trackIndex)
    {
        QRect trackRect(Theme::Metrics::TimelineMargin, TrackTop(trackIndex), width() - Theme::Metrics::TimelineMargin * 2, Theme::Metrics::TimelineTrackHeight);
        painter.fillRect(trackRect, Theme::Colours::Track);

        painter.setPen(QPen(Theme::Colours::AccentSelected, 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(trackRect);
    }

    void TimelineWidget::DrawRuler(QPainter& painter)
    {
        painter.setPen(Theme::Colours::Border);
        painter.drawLine(Theme::Metrics::TimelineMargin, Theme::Metrics::TimelineTopPadding + RulerHeight, width() - Theme::Metrics::TimelineMargin, Theme::Metrics::TimelineTopPadding + RulerHeight);

        TimelineViewport& viewport = m_StudioApp->GetSession().Viewport;
        for (auto time = viewport.Start; time <= viewport.Start + viewport.Duration; time += TickSpacing)
        {
            int x = TimeToX(time);
            painter.drawLine(x, RulerHeight - 5, x, RulerHeight);
            painter.drawText(x + 5, Theme::Metrics::TimelineTopPadding + RulerHeight - 8, Timecode::FormatTimeline(time));
        }
    }

    void TimelineWidget::DrawPlayhead(QPainter& painter)
    {
        std::chrono::milliseconds playhead = m_StudioApp->GetSession().Playback.Position;
        int x = TimeToX(playhead);

        QPolygon triangle;

        triangle << QPoint(x, Theme::Metrics::TimelineTopPadding + 8);
        triangle << QPoint(x - 6, Theme::Metrics::TimelineTopPadding);
        triangle << QPoint(x + 6, Theme::Metrics::TimelineTopPadding);

        painter.setBrush(Theme::Colours::Playhead);
        painter.setPen(Qt::NoPen);
        painter.drawPolygon(triangle);

        painter.setPen(QPen(Theme::Colours::Playhead, 2));
        painter.drawLine(x, Theme::Metrics::TimelineTopPadding + 8, x, height());
    }

    bool TimelineWidget::IsPreviewTrack(int trackIndex) const
    {
        const auto& drag = m_StudioApp->GetSubtitleEditor().DragState();
        return drag.Mode == DragMode::Move && trackIndex == m_StudioApp->TrackCount();
    }

    TimelineHit TimelineWidget::HitTest(const QPoint& point)
    {
        TimelineHit hit;

        if (!m_StudioApp->TracksAvailable())
        {
            return hit;
        }

        auto& tracks = m_StudioApp->GetSession().Tracks;
        for (int trackIndex = 0; trackIndex < static_cast<int>(tracks.size()); ++trackIndex)
        {
            auto& track = tracks[trackIndex];

            for (auto& subtitle : track.Subtitles)
            {
                if (!GetSubtitleRect(*subtitle, trackIndex).contains(point))
                {
                    continue;
                }

                hit.Type = TimelineHitType::Body;
                hit.Selection.TrackIndex = trackIndex;
                hit.Selection.Subtitle = subtitle.get();

                return hit;
            }
        }

        return hit;
    }

    QRect TimelineWidget::GetSubtitleRect(const Subtitle& subtitle, int displayTrack) const
    {
        int left = TimeToX(subtitle.Start);
        int right = TimeToX(subtitle.End);

        return QRect(left, TrackTop(displayTrack), right - left, Theme::Metrics::TimelineTrackHeight);
    }

    int TimelineWidget::TimeToX(std::chrono::milliseconds time) const
    {
        int usableWidth = width() - (Theme::Metrics::TimelineMargin * 2);
        double pixelsPerMillisecond = static_cast<double>(usableWidth) / m_StudioApp->GetSession().Viewport.Duration.count();

        auto relativeTime = time - m_StudioApp->GetSession().Viewport.Start;
        return Theme::Metrics::TimelineMargin + static_cast<int>(relativeTime.count() * pixelsPerMillisecond);
    }

    std::chrono::milliseconds TimelineWidget::XToTime(int x) const
    {
        int usableWidth = width() - (Theme::Metrics::TimelineMargin * 2);
        double millisecondsPerPixel = static_cast<double>(m_StudioApp->GetSession().Viewport.Duration.count()) / usableWidth;

        auto relativeTime = std::chrono::milliseconds(static_cast<long long>((x - Theme::Metrics::TimelineMargin) * millisecondsPerPixel));
        return m_StudioApp->GetSession().Viewport.Start + relativeTime;
    }

    int TimelineWidget::TrackTop(int trackIndex) const
    {
        return Theme::Metrics::TimelineTopPadding + RulerHeight + Theme::Metrics::TimelineTrackSpacing + trackIndex *
            (Theme::Metrics::TimelineTrackHeight + Theme::Metrics::TimelineTrackSpacing);
    }

    int TimelineWidget::YToTrack(int y) const
    {
        y -= Theme::Metrics::TimelineTopPadding;
        y -= RulerHeight;
        y -= Theme::Metrics::TimelineTrackSpacing;

        if (y < 0)
        {
            return 0;
        }

        int pitch = Theme::Metrics::TimelineTrackHeight + Theme::Metrics::TimelineTrackSpacing;
        int track = y / pitch;
        int maxTrack = static_cast<int>(m_StudioApp->TrackCount());
        return std::clamp(track, 0, maxTrack);
    }
}