#pragma once

#include "app/Application.h"

#include <QWidget>
#include <QPushButton>

namespace SubtitleStudio
{
	class PlaybackControlsWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit PlaybackControlsWidget(QWidget* parent = nullptr);

		void SetApplication(Application* app);

	private slots:
		void OnPlayButtonPressed();
		void OnPlayingStateChanged(bool playing);

	private:
		Application* m_StudioApp;
		QPushButton* m_PlayButton;
	};
}