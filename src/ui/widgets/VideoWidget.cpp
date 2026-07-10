#include "VideoWidget.h"

#include <QVBoxLayout>

namespace SubtitleStudio
{
    VideoWidget::VideoWidget(QWidget* parent)
        : QWidget(parent)
    {
        auto* layout = new QVBoxLayout(this);

        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        m_VideoWidget = new QVideoWidget(this);
        m_VideoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        layout->addWidget(m_VideoWidget);
    }

    QVideoWidget* VideoWidget::GetVideoWidget() const
    {
        return m_VideoWidget;
    }
}