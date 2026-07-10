#include "VideoPlayer.h"

namespace SubtitleStudio
{
	VideoPlayer::VideoPlayer(QObject* parent)
		: QObject(parent)
	{
		m_MediaPlayer = new QMediaPlayer(this);
		m_AudioOutput = new QAudioOutput(this);
		m_MediaPlayer->setAudioOutput(m_AudioOutput);

		connect(m_MediaPlayer, &QMediaPlayer::positionChanged, this, &VideoPlayer::OnPositionChanged);
		connect(m_MediaPlayer, &QMediaPlayer::playingChanged, this, &VideoPlayer::OnPlayingStateChanged);
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

	void VideoPlayer::SetVideoOutput(QVideoWidget* videoWidget)
	{
		m_MediaPlayer->setVideoOutput(videoWidget);
	}

	void VideoPlayer::OnPositionChanged(qint64 position)
	{
		emit PositionChanged(std::chrono::milliseconds(position));
	}

	void VideoPlayer::OnPlayingStateChanged(bool playing)
	{
		emit PlayingStateChanged(playing);
	}
}