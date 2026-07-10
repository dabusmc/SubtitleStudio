#include "PlaybackControlsWidget.h"

#include "ui/Theme.h"
#include "util/Timecode.h"

#include <QHBoxLayout>

namespace SubtitleStudio
{
    PlaybackControlsWidget::PlaybackControlsWidget(QWidget* parent)
        : QWidget(parent)
    {
        setFixedHeight(40);

        ApplyTheme();
        CreateLayout();
    }

    void PlaybackControlsWidget::SetApplication(Application* app)
    {
        m_StudioApp = app;

        connect(m_StudioApp, &Application::PlayingStateChanged, this, &PlaybackControlsWidget::OnPlayingStateChanged);
        connect(m_StudioApp, &Application::PlaybackPositionChanged, this, &PlaybackControlsWidget::OnPlaybackPositionChanged);

        UpdateTimeDisplay();
    }

    QPushButton* PlaybackControlsWidget::CreateButton(const QString& icon, const QString& tooltip)
    {
        auto* button = new QPushButton(this);

        button->setIcon(QIcon(icon));
        button->setIconSize(QSize(18, 18));
        button->setFixedSize(32, 32);

        button->setCursor(Qt::PointingHandCursor);
        button->setFocusPolicy(Qt::NoFocus);

        button->setStyleSheet(Styles::ToolButton());

        button->setToolTip(tooltip);

        return button;
    }

    void PlaybackControlsWidget::CreateLayout()
    {
        auto* layout = new QHBoxLayout(this);
        layout->setContentsMargins(Theme::Metrics::WindowMargin, 0, Theme::Metrics::WindowMargin, 0);
        layout->setSpacing(6);

        m_PreviousSubtitleButton = CreateButton(Icons::PreviousSubtitle, "Previous Subtitle");
        m_RewindButton = CreateButton(Icons::Rewind, "-1 Second");
        m_PlayPauseButton = CreateButton(Icons::Play, "Play / Pause");
        m_FastForwardButton = CreateButton(Icons::FastForward, "+1 Second");
        m_NextSubtitleButton = CreateButton(Icons::NextSubtitle, "Next Subtitle");

        m_TimeLabel = new QLabel("00:00:00,000 / 00:00:00,000", this);
        m_TimeLabel->setStyleSheet(Styles::TimeLabel());

        layout->addWidget(m_PreviousSubtitleButton);
        layout->addWidget(m_RewindButton);
        layout->addWidget(m_PlayPauseButton);
        layout->addWidget(m_FastForwardButton);
        layout->addWidget(m_NextSubtitleButton);

        layout->addStretch();

        layout->addWidget(m_TimeLabel);

        connect(m_PlayPauseButton, &QPushButton::clicked, this, &PlaybackControlsWidget::OnPlayPausePressed);
        connect(m_RewindButton, &QPushButton::clicked, this, &PlaybackControlsWidget::OnRewindPressed);
        connect(m_FastForwardButton, &QPushButton::clicked, this, &PlaybackControlsWidget::OnFastForwardPressed);
        connect(m_PreviousSubtitleButton, &QPushButton::clicked, this, &PlaybackControlsWidget::OnPreviousSubtitlePressed);
        connect(m_NextSubtitleButton, &QPushButton::clicked, this, &PlaybackControlsWidget::OnNextSubtitlePressed);
    }

    void PlaybackControlsWidget::ApplyTheme()
    {
        setAutoFillBackground(true);

        QPalette palette = this->palette();
        palette.setColor(QPalette::Window, Theme::Colours::Panel);
        setPalette(palette);
    }

    void PlaybackControlsWidget::UpdateTimeDisplay()
    {
        if (!m_StudioApp)
            return;

        const auto& playback = m_StudioApp->GetSession().Playback;

        m_TimeLabel->setText(Timecode::ToSRT(playback.Position) + " / " + Timecode::ToSRT(playback.Duration));
    }

    void PlaybackControlsWidget::OnPlayPausePressed()
    {
        m_StudioApp->PlayPause();
    }

    void PlaybackControlsWidget::OnRewindPressed()
    {
        m_StudioApp->SeekRelative(-std::chrono::seconds(1));
    }

    void PlaybackControlsWidget::OnFastForwardPressed()
    {
        m_StudioApp->SeekRelative(std::chrono::seconds(1));
    }

    void PlaybackControlsWidget::OnPreviousSubtitlePressed()
    {
        m_StudioApp->PreviousSubtitleBoundary();
    }

    void PlaybackControlsWidget::OnNextSubtitlePressed()
    {
        m_StudioApp->NextSubtitleBoundary();
    }

    void PlaybackControlsWidget::OnPlayingStateChanged(bool playing)
    {
        if (playing)
        {
            m_PlayPauseButton->setIcon(QIcon(Icons::Pause));
        }
        else
        {
            m_PlayPauseButton->setIcon(QIcon(Icons::Play));
        }
    }

    void PlaybackControlsWidget::OnPlaybackPositionChanged(std::chrono::milliseconds)
    {
        UpdateTimeDisplay();
    }
}