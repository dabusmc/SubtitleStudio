#include "VideoWidget.h"

#include "ui/Theme.h"

#include <QPainter>

namespace SubtitleStudio
{
    VideoWidget::VideoWidget(QWidget* parent)
        : QWidget(parent)
    {
    }

    void VideoWidget::SetApplication(Application* app)
    {
        m_StudioApp = app;

        connect(&app->GetVideoPlayer(), &VideoPlayer::VideoFrameChanged, this, &VideoWidget::OnVideoFrameChanged);
        connect(app, &Application::PlaybackPositionChanged, this, [this](std::chrono::milliseconds){
                update();
            });
    }

    void VideoWidget::paintEvent(QPaintEvent*)
    {
        QPainter painter(this);

        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.setRenderHint(QPainter::TextAntialiasing);

        painter.fillRect(rect(), Theme::Colours::Background);

        DrawFrame(painter);

        DrawSubtitle(painter);
    }

    void VideoWidget::DrawFrame(QPainter& painter)
    {
        if (m_CurrentFrame.isNull())
            return;

        QSize imageSize = m_CurrentFrame.size();
        imageSize.scale(size(), Qt::KeepAspectRatio);

        QRect target(QPoint((width() - imageSize.width()) / 2, (height() - imageSize.height()) / 2), imageSize);
        painter.drawImage(target, m_CurrentFrame);
    }

    void VideoWidget::DrawSubtitle(QPainter& painter)
    {
        Subtitle* subtitle = m_StudioApp->CurrentSubtitle();
        if (!subtitle)
            return;

        QFont font("Arial", Theme::Subtitle::FontSize);
        painter.setFont(font);

        // Subtitle drawing area
        QRect textRect(Theme::Subtitle::HorizontalMargin, height() - 180, width() - (Theme::Subtitle::HorizontalMargin * 2), 120);

        constexpr auto Alignment = Qt::AlignHCenter | Qt::AlignBottom | Qt::TextWordWrap;

        // Outline
        painter.setPen(Qt::black);

        static const QPoint offsets[] =
        {
            {-Theme::Subtitle::OutlineThickness, -Theme::Subtitle::OutlineThickness },
            {-Theme::Subtitle::OutlineThickness,  0 },
            {-Theme::Subtitle::OutlineThickness,  Theme::Subtitle::OutlineThickness },
            { 0, -Theme::Subtitle::OutlineThickness },
            { 0,  Theme::Subtitle::OutlineThickness },
            { Theme::Subtitle::OutlineThickness, -Theme::Subtitle::OutlineThickness },
            { Theme::Subtitle::OutlineThickness,  0 },
            { Theme::Subtitle::OutlineThickness,  Theme::Subtitle::OutlineThickness }
        };

        for (const QPoint& offset : offsets)
        {
            painter.drawText(textRect.translated(offset), Alignment, subtitle->Text);
        }

        // Main text
        painter.setPen(Qt::white);
        painter.drawText(textRect, Alignment, subtitle->Text);
    }

    void VideoWidget::OnVideoFrameChanged(const QImage& image)
    {
        m_CurrentFrame = image;

        update();
    }
}