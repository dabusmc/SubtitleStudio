#pragma once

#include <string>

#include "ui/widgets/VideoWidget.h"

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>

namespace SubtitleStudio
{
	class VideoPlayer : public QObject
	{
		Q_OBJECT

	public:
		explicit VideoPlayer(QObject* parent = nullptr);

		void Load(const std::string& file);
		void Play();
		void Pause();
		void Reset();
		void Seek();

		void SetVideoOutput(QVideoWidget* videoWidget);

	signals:
		void PositionChanged(std::chrono::milliseconds position);

	private slots:
		void OnPositionChanged(qint64 position);

	private:
		QMediaPlayer* m_MediaPlayer;
		QAudioOutput* m_AudioOutput;
	};
}