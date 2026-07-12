#pragma once

#include <chrono>

#include "app/Application.h"
#include "subtitle/Subtitle.h"
#include "timeline/TimelineDragState.h"
#include "timeline/TimelineHit.h"

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
		void mouseMoveEvent(QMouseEvent* event) override;
		void mouseReleaseEvent(QMouseEvent* event) override;

	private:
		void BeginMove(QMouseEvent* event, const SubtitleSelection& selection);
		void UpdateMove(QMouseEvent* event);
		void EndMove();

		void DrawTracks(QPainter& painter);
		void DrawTrack(QPainter& painter, const SubtitleTrack& track, int trackIndex);
		void DrawDraggedSubtitle(QPainter& painter);
		void DrawEmptyTrack(QPainter& painter, int trackIndex);
		void DrawRuler(QPainter& painter);
		void DrawPlayhead(QPainter& painter);

		bool IsPreviewTrack(int trackIndex) const;

		TimelineHit HitTest(const QPoint& point);
		QRect GetSubtitleRect(const Subtitle& subtitle, int displayTrack) const;

		int TimeToX(std::chrono::milliseconds time) const;
		std::chrono::milliseconds XToTime(int x) const;

		int TrackTop(int trackIndex) const;
		int YToTrack(int y) const;

	private:
		Application* m_StudioApp;
		TimelineDragState m_Drag;
	};
}