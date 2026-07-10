#pragma once

#include "subtitle/Subtitle.h"

#include <QWidget>

namespace SubtitleStudio
{
	class SubtitleEditorWidget : public QWidget
	{
		Q_OBJECT

	public:
		explicit SubtitleEditorWidget(QWidget* parent = nullptr);

		void SetSubtitle(Subtitle* subtitle);

	private:
		Subtitle* m_CurrentSubtitle;
	};
}