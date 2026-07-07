#include "VideoWidget.h"

#include <QPainter>

namespace SubtitleStudio
{
    VideoWidget::VideoWidget(QWidget* parent)
        : QWidget(parent)
    {
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }

    void VideoWidget::paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        painter.fillRect(rect(), QColor(40, 0, 0));
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, "Video Display");
    }
}