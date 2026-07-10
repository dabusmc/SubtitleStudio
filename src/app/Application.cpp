#include "Application.h"

#include "subtitle/SRTParser.h"

namespace SubtitleStudio
{
	Application::Application(QObject* parent)
		: QObject(parent), m_Session(), m_Player(this)
	{
		connect(&m_Player, &VideoPlayer::VideoLoaded, this, &Application::OnVideoLoaded);
		connect(&m_Player, &VideoPlayer::PositionChanged, this, &Application::OnPositionChanged);
		connect(&m_Player, &VideoPlayer::PlayingStateChanged, this, &Application::OnPlayingStateChanged);
	}

	void Application::OpenSubtitle(const std::string& path)
	{
		m_Session.Track = SRT::Load(path);
		m_Session.SubtitlePath = path;

		emit SessionChanged();
	}

	void Application::OpenVideo(const std::string& path)
	{
		m_Session.VideoPath = path;
		m_Player.Load(path);

		emit SessionChanged();
	}

	void Application::OnVideoLoaded(std::chrono::milliseconds duration)
	{
		m_Session.Playback.Duration = duration;
	}

	constexpr double FollowThreshold = 0.8;
	void Application::OnPositionChanged(std::chrono::milliseconds position)
	{
		m_Session.Playback.Position = position;

		if (m_Session.Playback.Position >= m_Session.Viewport.Start + (m_Session.Viewport.Duration * FollowThreshold))
		{
			m_Session.Viewport.Start += std::chrono::seconds(5);
		}

		emit PlaybackPositionChanged(m_Session.Playback.Position);
	}

	void Application::OnPlayingStateChanged(bool playing)
	{
		m_Session.Playback.Playing = playing;

		emit PlayingStateChanged(m_Session.Playback.Playing);
	}
}