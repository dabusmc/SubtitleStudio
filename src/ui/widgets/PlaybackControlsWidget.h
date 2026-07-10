#pragma once

#include "app/Application.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>

namespace SubtitleStudio
{
    class PlaybackControlsWidget : public QWidget
    {
        Q_OBJECT

    public:
        explicit PlaybackControlsWidget(QWidget* parent = nullptr);

        void SetApplication(Application* app);

    private:
        QPushButton* CreateButton(const QString& icon, const QString& tooltip);
        void CreateLayout();
        void ApplyTheme();
        void UpdateTimeDisplay();

    private slots:
        void OnPlayPausePressed();
        void OnRewindPressed();
        void OnFastForwardPressed();
        void OnPreviousSubtitlePressed();
        void OnNextSubtitlePressed();

        void OnPlayingStateChanged(bool playing);
        void OnPlaybackPositionChanged(std::chrono::milliseconds position);

    private:
        Application* m_StudioApp = nullptr;

        QPushButton* m_PreviousSubtitleButton = nullptr;
        QPushButton* m_RewindButton = nullptr;
        QPushButton* m_PlayPauseButton = nullptr;
        QPushButton* m_FastForwardButton = nullptr;
        QPushButton* m_NextSubtitleButton = nullptr;

        QLabel* m_TimeLabel = nullptr;
    };
}