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

	void Application::OnPositionChanged(std::chrono::milliseconds position)
	{
		m_Session.Playback.Position = position;
		emit PlaybackPositionChanged(m_Session.Playback.Position);
	}
}