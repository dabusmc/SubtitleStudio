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
        connect(m_StudioApp, &Application::PlaybackPositionChanged, this, [this](std::chrono::milliseconds)
            {
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
        DrawTrack(painter);
        DrawPlayhead(painter);
    }

    void TimelineWidget::mousePressEvent(QMouseEvent* event)
    {
        if (!m_StudioApp)
            return;

        TimelineHit hit = HitTest(event->pos());
        if (hit.Type == TimelineHitType::Body)
        {
            BeginMove(event, hit.Selection);
            update();
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
        if (m_Drag.Mode == DragMode::Move && (event->buttons() & Qt::LeftButton))
        {
            UpdateMove(event);
        }

        update();
    }

    void TimelineWidget::mouseReleaseEvent(QMouseEvent* event)
    {
        if (m_Drag.Mode != DragMode::None)
        {
            EndMove();
            update();
        }
    }

    void TimelineWidget::BeginMove(QMouseEvent* event, const SubtitleSelection& selection)
    {
        m_Drag.Mode = DragMode::Move;
        m_Drag.MouseStart = event->pos();
        m_Drag.Selection = selection;
        m_Drag.OriginalSubtitle = *selection.Subtitle;
    }

    void TimelineWidget::UpdateMove(QMouseEvent* event)
    {
        if (!m_Drag.Selection.Subtitle)
            return;

        auto startTime = XToTime(m_Drag.MouseStart.x());
        auto currentTime = XToTime(event->pos().x());

        auto delta = currentTime - startTime;
        if (m_Drag.OriginalSubtitle.Start + delta < std::chrono::milliseconds(0))
        {
            delta = -m_Drag.OriginalSubtitle.Start;
        }

        m_Drag.Selection.Subtitle->Start = m_Drag.OriginalSubtitle.Start + delta;
        m_Drag.Selection.Subtitle->End = m_Drag.OriginalSubtitle.End + delta;
    }

    void TimelineWidget::EndMove()
    {
        m_Drag.Mode = DragMode::None;

        if (m_StudioApp->TracksAvailable())
        {
            auto& subtitles = m_StudioApp->ActiveTrack().Subtitles;

            std::sort(subtitles.begin(), subtitles.end(), [](const Subtitle& lhs, const Subtitle& rhs) {
                return lhs.Start < rhs.Start;
                });
        }
    }

    void TimelineWidget::DrawTrack(QPainter& painter)
    {
        QRect trackRect(Theme::Metrics::TimelineMargin, TrackTop, width() - Theme::Metrics::TimelineMargin * 2, Theme::Metrics::TimelineTrackHeight);
        painter.fillRect(trackRect, Theme::Colours::Track);

        const std::vector<Subtitle>* subtitles = nullptr;
        if (m_StudioApp->TracksAvailable())
        {
            subtitles = &m_StudioApp->ActiveTrack().Subtitles;
        }

        if (!subtitles)
        {
            return;
        }

        TimelineViewport& viewport = m_StudioApp->GetSession().Viewport;
        for (const auto& subtitle : *subtitles)
        {
            if (subtitle.End < viewport.Start)
                continue;

            if (subtitle.Start > viewport.Start + viewport.Duration)
                continue;

            QRect box = GetSubtitleRect(subtitle);

            painter.save();
            painter.setClipRect(trackRect);

            painter.setBrush(m_Drag.Selection.Subtitle == &subtitle ? Theme::Colours::AccentSelected : Theme::Colours::Accent);

            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(box, Theme::Metrics::BorderRadius, Theme::Metrics::BorderRadius);

            painter.setPen(Theme::Colours::Text);
            painter.drawText(box, Qt::AlignCenter, subtitle.Text);
            
            painter.restore();
        }
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
                if (!GetSubtitleRect(subtitle).contains(point))
                {
                    continue;
                }

                hit.Type = TimelineHitType::Body;
                hit.Selection.TrackIndex = trackIndex;
                hit.Selection.Subtitle = &subtitle;

                return hit;
            }
        }

        return hit;
    }

    QRect TimelineWidget::GetSubtitleRect(const Subtitle& subtitle) const
    {
        int left = TimeToX(subtitle.Start);
        int right = TimeToX(subtitle.End);
        return QRect(left, TrackTop, right - left, Theme::Metrics::TimelineTrackHeight);
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
}