#pragma once

#include <QWidget>

namespace SubtitleStudio
{
	class VideoWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit VideoWidget(QWidget* parent = nullptr);

	protected:
		void paintEvent(QPaintEvent* event) override;
	};
}