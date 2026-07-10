#pragma once

#include <chrono>

#include "app/Application.h"

#include <QWidget>

namespace SubtitleStudio
{
	class TimelineWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit TimelineWidget(QWidget* parent = nullptr);

		void SetApplication(Application* app) { m_StudioApp = app; }

	protected:
		void paintEvent(QPaintEvent* event) override;

	private:
		int TimeToX(std::chrono::milliseconds time) const;

	private:
		Application* m_StudioApp;
	};
}