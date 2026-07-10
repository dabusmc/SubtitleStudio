#pragma once

#include <string>

#include <QObject>
#include <QVideoSink>
#include <QVideoFrame>
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

	signals:
		void VideoLoaded(std::chrono::milliseconds duration);
		void PositionChanged(std::chrono::milliseconds position);
		void PlayingStateChanged(bool playing);
		void VideoFrameChanged(const QImage&);

	private slots:
		void OnMediaStatusChanged(QMediaPlayer::MediaStatus status);
		void OnPositionChanged(qint64 position);
		void OnPlayingStateChanged(bool playing);
		void OnVideoFrameChanged(const QVideoFrame& frame);

	private:
		QMediaPlayer* m_MediaPlayer;
		QAudioOutput* m_AudioOutput;
		QVideoSink* m_VideoSink;
	};
}