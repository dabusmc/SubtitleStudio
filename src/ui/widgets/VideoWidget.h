#pragma once

#include "app/Application.h"

#include <QWidget>
#include <QImage>

namespace SubtitleStudio
{
	class VideoWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit VideoWidget(QWidget* parent = nullptr);

		void SetApplication(Application* app);

	protected:
		void paintEvent(QPaintEvent*) override;

	private:
		void DrawFrame(QPainter&);
		void DrawSubtitle(QPainter&);

	private slots:
		void OnVideoFrameChanged(const QImage&);

	private:
		Application* m_StudioApp = nullptr;
		QImage m_CurrentFrame;
	};
}