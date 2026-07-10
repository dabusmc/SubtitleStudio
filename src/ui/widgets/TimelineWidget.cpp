#include "TimelineWidget.h"

#include <QPainter>

namespace SubtitleStudio
{
    TimelineWidget::TimelineWidget(QWidget* parent)
        : QWidget(parent), m_StudioApp(nullptr)
    {
        setFixedHeight(150);
    }

    constexpr int RulerHeight = 20;
    constexpr int TrackY = 10;
    constexpr int SubtitleVSpacing = 25;
    constexpr int TrackHeight = 30;
    void TimelineWidget::paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        painter.fillRect(rect(), QColor(40, 40, 40));

        if (!m_StudioApp)
            return;

        const auto& subtitles = m_StudioApp->GetSession().Track.Subtitles;

        int y = TrackY;
        painter.setPen(Qt::white);
        for (const auto& subtitle : subtitles)
        {
            int left = TimeToX(subtitle.Start);
            int right = TimeToX(subtitle.End);

            QRect box(left, y, right - left, 20);

            painter.fillRect(box, QColor(70, 150, 255));
            painter.drawText(box, Qt::AlignCenter, subtitle.Text);

            y += SubtitleVSpacing;
        }
    }

    constexpr auto VisibleDuration = std::chrono::minutes(1);
    int TimelineWidget::TimeToX(std::chrono::milliseconds time) const
    {
        auto visibleDuration = std::chrono::duration_cast<std::chrono::milliseconds>(VisibleDuration);
        double pixelsPerMillisecond = static_cast<double>(width()) / visibleDuration.count();
        return static_cast<int>(time.count() * pixelsPerMillisecond);
    }
}