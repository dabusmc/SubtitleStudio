#pragma once

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

		QLineEdit* m_StartTimeValue;
		QLineEdit* m_EndTimeValue;
		QLineEdit* m_DurationValue;
		QPlainTextEdit* m_TextValue;
	};
}