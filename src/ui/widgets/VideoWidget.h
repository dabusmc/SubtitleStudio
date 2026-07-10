#pragma once

#include <QWidget>
#include <QVideoWidget>

namespace SubtitleStudio
{
	class VideoWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit VideoWidget(QWidget* parent = nullptr);

		QVideoWidget* GetVideoWidget() const;

	private:
		QVideoWidget* m_VideoWidget;
	};
}