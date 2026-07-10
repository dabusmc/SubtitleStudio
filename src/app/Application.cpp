#include "Application.h"

#include "subtitle/SRTParser.h"

namespace SubtitleStudio
{
	Application::Application(QObject* parent)
		: QObject(parent), m_Session(), m_Player(this)
	{
		connect(&m_Player, &VideoPlayer::PositionChanged, this, &Application::OnPositionChanged);
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
}