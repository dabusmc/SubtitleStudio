#include "SubtitleEditorWidget.h"

#include <QLabel>
#include <QVBoxLayout>

namespace SubtitleStudio
{
	SubtitleEditorWidget::SubtitleEditorWidget(QWidget* parent)
		: QWidget(parent)
	{
		QVBoxLayout* layout = new QVBoxLayout(this);

		QLabel* label = new QLabel("Subtitle Editor!!!!", this);
		layout->addWidget(label);

		setLayout(layout);
	}

	void SubtitleEditorWidget::SetSubtitle(Subtitle* subtitle)
	{
		// TODO: Close current subtitle if one is open

		m_CurrentSubtitle = subtitle;

		// TODO: Update subtitle information
	}
}