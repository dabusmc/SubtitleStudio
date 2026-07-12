#pragma once

#include "app/Application.h"
#include "subtitle/Subtitle.h"

#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include <QPlainTextEdit>

namespace SubtitleStudio
{
	class SubtitleEditorWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit SubtitleEditorWidget(QWidget* parent = nullptr);

		void SetApplication(Application* app);
		void SetSubtitle(Subtitle* subtitle);

	signals:
		void SubtitleChanged();

	private:
		void CreateLayout();
		QWidget* CreateTimingCard();
		QWidget* CreateTextCard();

	private slots:
		void OnTextChanged();

	private:
		Subtitle* m_CurrentSubtitle;
		Application* m_StudioApp;

		QLineEdit* m_StartTimeValue;
		QLineEdit* m_EndTimeValue;
		QLineEdit* m_DurationValue;
		QPlainTextEdit* m_TextValue;
	};
}