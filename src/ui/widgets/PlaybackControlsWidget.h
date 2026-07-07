#pragma once

#include <QWidget>

namespace SubtitleStudio
{
	class PlaybackControlsWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit PlaybackControlsWidget(QWidget* parent = nullptr);

	protected:
		void paintEvent(QPaintEvent* event) override;
	};
}