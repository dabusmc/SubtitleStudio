#include "PlaybackControlsWidget.h"

#include <QPainter>

namespace SubtitleStudio
{
    PlaybackControlsWidget::PlaybackControlsWidget(QWidget* parent)
        : QWidget(parent)
    {
        setFixedHeight(40);
    }

    void PlaybackControlsWidget::paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        painter.fillRect(rect(), QColor(0, 40, 0));
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, "Playback Controls Display");
    }
}