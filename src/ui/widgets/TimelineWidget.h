#pragma once

#include <chrono>

#include "app/Application.h"
#include "subtitle/Subtitle.h"

#include <QRect>
#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>

namespace SubtitleStudio
{
	struct TimelineViewport
	{
		std::chrono::milliseconds Start = std::chrono::milliseconds(0);
		std::chrono::milliseconds Duration = std::chrono::minutes(1);
	};

	class TimelineWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit TimelineWidget(QWidget* parent = nullptr);

		void SetApplication(Application* app);

	protected:
		void paintEvent(QPaintEvent* event) override;
		void mousePressEvent(QMouseEvent* event) override;

	private:
		void DrawTrack(QPainter& painter);
		void DrawRuler(QPainter& painter);
		void DrawPlayhead(QPainter& painter);

		Subtitle* SubtitleAt(const QPoint& point);
		QRect GetSubtitleRect(const Subtitle& subtitle) const;

		int TimeToX(std::chrono::milliseconds time) const;
		std::chrono::milliseconds XToTime(int x) const;

	private:
		Application* m_StudioApp;
		TimelineViewport m_Viewport;
	};
}