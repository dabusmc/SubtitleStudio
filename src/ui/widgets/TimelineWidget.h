#pragma once

#include <QWidget>

namespace SubtitleStudio
{
	class TimelineWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit TimelineWidget(QWidget* parent = nullptr);

	protected:
		void paintEvent(QPaintEvent* event) override;
	};
}