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

	void Application::PlayPause()
	{
		if (m_Session.Playback.Playing)
		{
			m_Player.Pause();
		}
		else
		{
			m_Player.Play();
		}
	}

	void Application::SeekRelative(std::chrono::milliseconds amount)
	{
		auto position = m_Session.Playback.Position + amount;
		position = std::clamp(position, std::chrono::milliseconds(0), m_Session.Playback.Duration);
		m_Player.Seek(position);
	}

	void Application::PreviousSubtitleBoundary()
	{
		int currentIndex = CurrentSubtitleIndex();

		if (currentIndex != -1)
		{
			Subtitle& current = m_Session.Track.Subtitles[currentIndex];

			if (m_Session.Playback.Position > current.Start)
			{
				m_Player.Seek(current.Start);
				return;
			}

			if (currentIndex > 0)
			{
				m_Player.Seek(m_Session.Track.Subtitles[currentIndex - 1].Start);
			}

			return;
		}

		int previousIndex = PreviousSubtitleIndex();

		if (previousIndex != -1)
		{
			m_Player.Seek(m_Session.Track.Subtitles[previousIndex].Start);
		}
	}

	void Application::NextSubtitleBoundary()
	{
		int currentIndex = CurrentSubtitleIndex();

		if (currentIndex != -1)
		{
			Subtitle& current = m_Session.Track.Subtitles[currentIndex];

			if (m_Session.Playback.Position < current.End)
			{
				m_Player.Seek(current.End);
				return;
			}

			if (currentIndex + 1 < static_cast<int>(m_Session.Track.Subtitles.size()))
			{
				m_Player.Seek(m_Session.Track.Subtitles[currentIndex + 1].Start);
			}

			return;
		}

		int nextIndex = NextSubtitleIndex();

		if (nextIndex != -1)
		{
			m_Player.Seek(m_Session.Track.Subtitles[nextIndex].Start);
		}
	}

	int Application::CurrentSubtitleIndex() const
	{
		return SubtitleIndexAt(m_Session.Playback.Position);
	}

	int Application::PreviousSubtitleIndex() const
	{
		const auto& subtitles = m_Session.Track.Subtitles;
		auto position = m_Session.Playback.Position;

		for (int i = static_cast<int>(subtitles.size()) - 1; i >= 0; --i)
		{
			if (subtitles[i].Start < position)
			{
				return i;
			}
		}

		return -1;
	}

	int Application::NextSubtitleIndex() const
	{
		const auto& subtitles = m_Session.Track.Subtitles;
		auto position = m_Session.Playback.Position;

		for (int i = 0; i < static_cast<int>(subtitles.size()); ++i)
		{
			if (subtitles[i].Start > position)
			{
				return i;
			}
		}

		return -1;
	}

	int Application::SubtitleIndexAt(std::chrono::milliseconds position) const
	{
		const auto& subtitles = m_Session.Track.Subtitles;

		for (int i = 0; i < static_cast<int>(subtitles.size()); ++i)
		{
			const auto& subtitle = subtitles[i];

			if (subtitle.Start <= position &&
				position < subtitle.End)
			{
				return i;
			}
		}

		return -1;
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