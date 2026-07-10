#include "PlaybackControlsWidget.h"

#include <QVBoxLayout>

namespace SubtitleStudio
{
    PlaybackControlsWidget::PlaybackControlsWidget(QWidget* parent)
        : QWidget(parent)
    {
        setFixedHeight(40);

        // NOTE: This entire UI is temporary and just for easily handling video playback
        QVBoxLayout* layout = new QVBoxLayout(this);

        m_PlayButton = new QPushButton("Play", this);
        connect(m_PlayButton, &QPushButton::pressed, this, &PlaybackControlsWidget::OnPlayButtonPressed);
        layout->addWidget(m_PlayButton);

        setLayout(layout);
    }

    void PlaybackControlsWidget::SetApplication(Application* app)
    {
        m_StudioApp = app;
        connect(m_StudioApp, &Application::PlayingStateChanged, this, &PlaybackControlsWidget::OnPlayingStateChanged);
    }

    void PlaybackControlsWidget::OnPlayButtonPressed()
    {
        if (m_StudioApp->GetSession().Playback.Playing)
        {
            // Stop
            m_StudioApp->GetVideoPlayer().Pause();
        }
        else
        {
            // Play
            m_StudioApp->GetVideoPlayer().Play();
        }
    }

    void PlaybackControlsWidget::OnPlayingStateChanged(bool playing)
    {
        if (playing)
        {
            m_PlayButton->setText("Stop");
        }
        else
        {
            m_PlayButton->setText("Play");
        }

        update();
    }
}