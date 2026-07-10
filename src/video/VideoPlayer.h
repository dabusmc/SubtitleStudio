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
		void Seek(std::chrono::milliseconds position);

		void SetVideoOutput(QVideoWidget* videoWidget);

	signals:
		void PositionChanged(std::chrono::milliseconds position);
		void PlayingStateChanged(bool playing);

	private slots:
		void OnPositionChanged(qint64 position);
		void OnPlayingStateChanged(bool playing);

	private:
		QMediaPlayer* m_MediaPlayer;
		QAudioOutput* m_AudioOutput;
	};
}