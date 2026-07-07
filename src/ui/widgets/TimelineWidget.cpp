#include "TimelineWidget.h"

#include <QPainter>

namespace SubtitleStudio
{
    TimelineWidget::TimelineWidget(QWidget* parent)
        : QWidget(parent)
    {
        setFixedHeight(150);
    }

    void TimelineWidget::paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        painter.fillRect(rect(), QColor(0, 0, 40));
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, "Timeline Display");
    }
}