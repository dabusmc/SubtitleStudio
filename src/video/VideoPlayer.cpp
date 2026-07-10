#include "VideoPlayer.h"

namespace SubtitleStudio
{
	VideoPlayer::VideoPlayer(QObject* parent)
		: QObject(parent)
	{
		m_MediaPlayer = new QMediaPlayer(this);
		m_AudioOutput = new QAudioOutput(this);
		m_MediaPlayer->setAudioOutput(m_AudioOutput);

		m_VideoSink = new QVideoSink(this);
		m_MediaPlayer->setVideoSink(m_VideoSink);

		connect(m_MediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &VideoPlayer::OnMediaStatusChanged);
		connect(m_MediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::OnPositionChanged);
		connect(m_MediaPlayer, &QMediaPlayer::playingChanged, this, &VideoPlayer::OnPlayingStateChanged);
		connect(m_VideoSink, &QVideoSink::videoFrameChanged, this, &VideoPlayer::OnVideoFrameChanged);
	}

	void VideoPlayer::Load(const std::string& file)
	{
		m_MediaPlayer->setSource(QUrl::fromLocalFile(QString::fromUtf8(file)));

		// For now, we instantly start playing when loading
		Play();
	}

	void VideoPlayer::Play()
	{
		m_MediaPlayer->play();
	}

	void VideoPlayer::Pause()
	{
		m_MediaPlayer->pause();
	}

	void VideoPlayer::Reset()
	{
		m_MediaPlayer->stop();
	}

	void VideoPlayer::Seek(std::chrono::milliseconds position)
	{
		m_MediaPlayer->setPosition(static_cast<qint64>(position.count()));
	}

	void VideoPlayer::OnMediaStatusChanged(QMediaPlayer::MediaStatus status)
	{
		if (status == QMediaPlayer::MediaStatus::LoadedMedia)
		{
			emit VideoLoaded(std::chrono::milliseconds(m_MediaPlayer->duration()));
		}
	}

	void VideoPlayer::OnPositionChanged(qint64 position)
	{
		emit PositionChanged(std::chrono::milliseconds(position));
	}

	void VideoPlayer::OnPlayingStateChanged(bool playing)
	{
		emit PlayingStateChanged(playing);
	}

	void VideoPlayer::OnVideoFrameChanged(const QVideoFrame& frame)
	{
		if (!frame.isValid())
			return;

		QImage image = frame.toImage();

		if (image.isNull())
			return;

		emit VideoFrameChanged(image);
	}
}