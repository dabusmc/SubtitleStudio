#include "TimelineWidget.h"

#include <algorithm>

#include "util/Timecode.h"

#include <QPainter>

namespace SubtitleStudio
{
    namespace
    {
        constexpr int TimelineMargin = 10;
        constexpr int TopPadding = 10;

        constexpr int RulerHeight = 20;
        constexpr auto TickSpacing = std::chrono::seconds(5);
        
        constexpr int TrackSpacing = 10;
        constexpr int TrackHeight = 30;
        constexpr int TrackTop = TopPadding + RulerHeight + TrackSpacing;

        constexpr QColor NormalSubtitleColour(70, 150, 255);
        constexpr QColor SelectedSubtitleColour(255, 180, 60);
    }

    TimelineWidget::TimelineWidget(QWidget* parent)
        : QWidget(parent), m_StudioApp(nullptr), m_SelectedSubtitle(nullptr)
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
        painter.fillRect(rect(), QColor(40, 40, 40));

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

        if (Subtitle* subtitle = SubtitleAt(event->pos()))
        {
            m_SelectedSubtitle = subtitle;
            update();
            return;
        }

        TimelineViewport& viewport = m_StudioApp->GetSession().Viewport;
        auto seekPosition = std::clamp(XToTime(event->pos().x()), viewport.Start, viewport.Start + viewport.Duration);
        m_StudioApp->GetVideoPlayer().Seek(seekPosition);
        update();
    }

    void TimelineWidget::DrawTrack(QPainter& painter)
    {
        QRect trackRect(TimelineMargin, TrackTop, width() - TimelineMargin * 2, TrackHeight);
        painter.fillRect(trackRect, QColor(55, 55, 55));

        const auto& subtitles = m_StudioApp->GetSession().Track.Subtitles;

        TimelineViewport& viewport = m_StudioApp->GetSession().Viewport;
        for (const auto& subtitle : subtitles)
        {
            if (subtitle.End < viewport.Start)
                continue;

            if (subtitle.Start > viewport.Start + viewport.Duration)
                continue;

            QRect box = GetSubtitleRect(subtitle);

            painter.save();
            painter.setClipRect(trackRect);

            painter.setBrush(&subtitle == m_SelectedSubtitle ? SelectedSubtitleColour : NormalSubtitleColour);

            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(box, 3, 3);

            painter.setPen(Qt::white);
            painter.drawText(box, Qt::AlignCenter, subtitle.Text);
            
            painter.restore();
        }
    }

    void TimelineWidget::DrawRuler(QPainter& painter)
    {
        painter.setPen(QColor(90, 90, 90));
        painter.drawLine(TimelineMargin, TopPadding + RulerHeight, width() - TimelineMargin, TopPadding + RulerHeight);

        TimelineViewport& viewport = m_StudioApp->GetSession().Viewport;
        for (auto time = viewport.Start; time <= viewport.Start + viewport.Duration; time += TickSpacing)
        {
            int x = TimeToX(time);
            painter.drawLine(x, RulerHeight - 5, x, RulerHeight);
            painter.drawText(x + 5, TopPadding + RulerHeight - 8, Timecode::FormatTimeline(time));
        }
    }

    void TimelineWidget::DrawPlayhead(QPainter& painter)
    {
        std::chrono::milliseconds playhead = m_StudioApp->GetSession().Playback.Position;
        int x = TimeToX(playhead);

        QPolygon triangle;

        triangle << QPoint(x, TopPadding + 8);
        triangle << QPoint(x - 6, TopPadding);
        triangle << QPoint(x + 6, TopPadding);

        painter.setBrush(Qt::red);
        painter.setPen(Qt::NoPen);
        painter.drawPolygon(triangle);

        painter.setPen(QPen(Qt::red, 2));
        painter.drawLine(x, TopPadding + 8, x, height());
    }

    Subtitle* TimelineWidget::SubtitleAt(const QPoint& point)
    {
        auto& subtitles = m_StudioApp->GetSession().Track.Subtitles;

        for (auto& subtitle : subtitles)
        {
            if (GetSubtitleRect(subtitle).contains(point))
            {
                return &subtitle;
            }
        }

        return nullptr;
    }

    QRect TimelineWidget::GetSubtitleRect(const Subtitle& subtitle) const
    {
        int left = TimeToX(subtitle.Start);
        int right = TimeToX(subtitle.End);
        return QRect(left, TrackTop, right - left, TrackHeight);
    }

    int TimelineWidget::TimeToX(std::chrono::milliseconds time) const
    {
        int usableWidth = width() - (TimelineMargin * 2);
        double pixelsPerMillisecond = static_cast<double>(usableWidth) / m_StudioApp->GetSession().Viewport.Duration.count();

        auto relativeTime = time - m_StudioApp->GetSession().Viewport.Start;
        return TimelineMargin + static_cast<int>(relativeTime.count() * pixelsPerMillisecond);
    }

    std::chrono::milliseconds TimelineWidget::XToTime(int x) const
    {
        int usableWidth = width() - (TimelineMargin * 2);
        double millisecondsPerPixel = static_cast<double>(m_StudioApp->GetSession().Viewport.Duration.count()) / usableWidth;

        auto relativeTime = std::chrono::milliseconds(static_cast<long long>((x - TimelineMargin) * millisecondsPerPixel));
        return m_StudioApp->GetSession().Viewport.Start + relativeTime;
    }
}