#include "SubtitleEditorWidget.h"

#include "ui/Theme.h"
#include "util/Timecode.h"

#include <QLabel>
#include <QVBoxLayout>

namespace SubtitleStudio
{
	SubtitleEditorWidget::SubtitleEditorWidget(QWidget* parent)
		: QWidget(parent)
	{
		setAutoFillBackground(true);

		QPalette palette = this->palette();
		palette.setColor(QPalette::Window, Theme::Colours::Background);
		setPalette(palette);

		CreateLayout();
	}

	void SubtitleEditorWidget::CreateLayout()
	{
		QVBoxLayout* layout = new QVBoxLayout(this);
		layout->setSpacing(Theme::Metrics::CardSpacing);
		layout->setContentsMargins(Theme::Metrics::WindowMargin, Theme::Metrics::WindowMargin, Theme::Metrics::WindowMargin, Theme::Metrics::WindowMargin);

		QWidget* timingCard = CreateTimingCard();
		QWidget* textCard = CreateTextCard();

		layout->addWidget(timingCard);
		layout->addWidget(textCard, 1);
		layout->addStretch();

		setLayout(layout);
	}

	QWidget* SubtitleEditorWidget::CreateTimingCard()
	{
		QWidget* timingCard = new QWidget(this);
		timingCard->setStyleSheet(Styles::Card());

		QLabel* startLabel = new QLabel("Start", timingCard);
		startLabel->setStyleSheet(Styles::Label());
		QLabel* endLabel = new QLabel("End", timingCard);
		endLabel->setStyleSheet(Styles::Label());
		QLabel* durationLabel = new QLabel("Duration", timingCard);
		durationLabel->setStyleSheet(Styles::Label());

		m_StartTimeValue = new QLineEdit("0", timingCard);
		m_StartTimeValue->setReadOnly(true);
		m_StartTimeValue->setStyleSheet(Styles::ReadOnlyField());

		m_EndTimeValue = new QLineEdit("0", timingCard);
		m_EndTimeValue->setReadOnly(true);
		m_EndTimeValue->setStyleSheet(Styles::ReadOnlyField());

		m_DurationValue = new QLineEdit("0", timingCard);
		m_DurationValue->setReadOnly(true);
		m_DurationValue->setStyleSheet(Styles::ReadOnlyField());

		QGridLayout* timingLayout = new QGridLayout(timingCard);
		timingLayout->setContentsMargins(Theme::Metrics::CardPadding, Theme::Metrics::CardPadding, Theme::Metrics::CardPadding, Theme::Metrics::CardPadding);

		timingLayout->addWidget(startLabel, 0, 0);
		timingLayout->addWidget(m_StartTimeValue, 0, 1);

		timingLayout->addWidget(endLabel, 1, 0);
		timingLayout->addWidget(m_EndTimeValue, 1, 1);

		timingLayout->addWidget(durationLabel, 2, 0);
		timingLayout->addWidget(m_DurationValue, 2, 1);

		return timingCard;
	}

	QWidget* SubtitleEditorWidget::CreateTextCard()
	{
		QWidget* textCard = new QWidget(this);
		textCard->setStyleSheet(Styles::Card());

		QLabel* textLabel = new QLabel("Text", textCard);
		textLabel->setStyleSheet(Styles::Label());
		m_TextValue = new QPlainTextEdit("Subtitle Not Selected!", textCard);
		m_TextValue->setStyleSheet(Styles::TextEditor());

		QVBoxLayout* textLayout = new QVBoxLayout(textCard);
		textLayout->setContentsMargins(Theme::Metrics::CardPadding, Theme::Metrics::CardPadding, Theme::Metrics::CardPadding, Theme::Metrics::CardPadding);
		textLayout->addWidget(textLabel);
		textLayout->addWidget(m_TextValue);

		return textCard;
	}

	void SubtitleEditorWidget::SetSubtitle(Subtitle* subtitle)
	{
		m_CurrentSubtitle = subtitle;

		if (!m_CurrentSubtitle)
		{
			m_StartTimeValue->setText("0");
			m_EndTimeValue->setText("0");
			m_DurationValue->setText("0");
			m_TextValue->setPlainText("");
			return;
		}

		m_StartTimeValue->setText(Timecode::ToSRT(m_CurrentSubtitle->Start));
		m_EndTimeValue->setText(Timecode::ToSRT(m_CurrentSubtitle->End));
		m_DurationValue->setText(Timecode::ToSRT(m_CurrentSubtitle->End - m_CurrentSubtitle->Start));
		m_TextValue->setPlainText(m_CurrentSubtitle->Text);
	}
}