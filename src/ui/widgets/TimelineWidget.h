#pragma once

#include <chrono>

#include "app/Application.h"
#include "subtitle/Subtitle.h"
#include "subtitle/SubtitleSelection.h"

#include <QRect>
#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>

namespace SubtitleStudio
{
	class TimelineWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit TimelineWidget(QWidget* parent = nullptr);

		void SetApplication(Application* app);

	signals:
		void SubtitlePropertiesOpen(Subtitle* subtitle);

	protected:
		void paintEvent(QPaintEvent* event) override;
		void mousePressEvent(QMouseEvent* event) override;
		void mouseDoubleClickEvent(QMouseEvent* event) override;

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
		SubtitleSelection m_SelectedSubtitle;
	};
}