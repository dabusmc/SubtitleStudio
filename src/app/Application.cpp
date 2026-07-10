/*

TODO List (No Particular Order)
-------------------------------
# SRT Saving
# Changing the Start/End of a subtitle with drag boxes
# Creating New Subtitles
# Allowing users to open a subtitle + video at the same time
# Saving the last opened folder when selecting a subtitle or video
# Allowing users to drag + drop subtitles and video files
# Make the spacebar pause/play
# Make the left/right arrow keys rewind and fast-forward
# Customisation options for Subtitles display
# Comments/Code Cleanup
# License Comments at the top of Source Files

*/

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

	Subtitle* Application::CurrentSubtitle()
	{
		auto& subtitles = m_Session.Track.Subtitles;
		auto index = CurrentSubtitleIndex();

		if (index >= 0 && index < static_cast<int>(subtitles.size()))
		{
			return &subtitles[index];
		}

		return nullptr;
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

			if (subtitle.Start <= position && position < subtitle.End)
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

	constexpr double SafeLeft = 0.2;
	constexpr double SafeRight = 1.0 - SafeLeft;
	void Application::OnPositionChanged(std::chrono::milliseconds position)
	{
		m_Session.Playback.Position = position;

		auto leftBoundary = m_Session.Viewport.Start + (m_Session.Viewport.Duration * SafeLeft);
		auto rightBoundary = m_Session.Viewport.Start + (m_Session.Viewport.Duration * SafeRight);

		if (position > rightBoundary)
		{
			auto viewportOffset = std::chrono::duration_cast<std::chrono::milliseconds>(m_Session.Viewport.Duration * SafeRight);
			m_Session.Viewport.Start = position - viewportOffset;
		}
		else if (position < leftBoundary)
		{
			auto viewportOffset = std::chrono::duration_cast<std::chrono::milliseconds>(m_Session.Viewport.Duration * SafeLeft);
			m_Session.Viewport.Start = position - viewportOffset;
		}

		m_Session.Viewport.Start = std::max(std::chrono::milliseconds(0), m_Session.Viewport.Start);

		emit PlaybackPositionChanged(m_Session.Playback.Position);
	}

	void Application::OnPlayingStateChanged(bool playing)
	{
		m_Session.Playback.Playing = playing;

		emit PlayingStateChanged(m_Session.Playback.Playing);
	}
}