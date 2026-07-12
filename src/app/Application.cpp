/*

TODO List (No Particular Order)
-------------------------------
# Changing the Start/End of a subtitle with drag boxes
# Creating New Subtitles
# Multiple Tracks
# Multi-selection
	# Ctrl + Click
	# Shift + Click
	# Select All
# Moving subtitles by dragging
	# Snap subtitle edges to neighbouring subtitles
	# Moving up and down changes track
# Deleting Subtitles
# Splitting a subtitle
	# Merging subtitles
# Prevent invalid subtitles
	# End >= Start
	# Prevent negative timestamps
# Undo/Redo Functionality
# Allowing users to open a subtitle + video at the same time
# Saving the last opened folder when selecting a subtitle or video
# Allowing users to drag + drop subtitles and video files
# Fix weird display issue with ruler on timeline
# Give the keyboard functionality
	# Make the spacebar pause/play
	# Make the left/right arrow keys rewind and fast-forward
	# Make shift + left/right arrow keys go to previous and next subtitle
# Customisation options for Subtitles display
# Comments/Code Cleanup
# License Comments at the top of Source Files
# Closing Videos and Subtitles individually
	# Allowing videos to be closed mid-play
# Timeline Movement
	# Timeline zoom with Ctrl + mouse wheel
	# Timeline horizontal scrolling with mouse wheel
	# Auto-scroll toggle (follow playback)
# Better timeline subtitle visuals
# Recent files
# Auto Save
# Prompt before closing unsaved work
# "Modified" indicator in title bar
# Application icon
# Support for Foreign Languages both in terms of subtitles but also the application itself
# See if there's a cheap way to make the video less choppy
# Properly update view subtitles properties
# Make sure subtitles properties updates when moving/changing subtitle time

# Handling other subtitle formats (maybe)
	# Export to other formats
# Multiple subtitle tracks (maybe)
# Light/Dark themes (maybe)

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

	bool Application::TracksAvailable() const
	{
		return !m_Session.Tracks.empty();
	}

	SubtitleTrack& Application::CreateTrack()
	{
		m_Session.Tracks.emplace_back();

		auto& track = m_Session.Tracks.back();

		track.Name = "Track " + std::to_string(m_Session.Tracks.size());

		if (m_Session.ActiveTrack == -1)
		{
			m_Session.ActiveTrack = 0;
		}

		emit SessionChanged();

		return track;
	}

	void Application::RemoveTrack(int index)
	{
		if (index < 0 || index >= static_cast<int>(m_Session.Tracks.size()))
			return;

		m_Session.Tracks.erase(m_Session.Tracks.begin() + index);

		if (m_Session.Tracks.empty())
		{
			m_Session.ActiveTrack = -1;
		}
		else if (m_Session.ActiveTrack >= static_cast<int>(m_Session.Tracks.size()))
		{
			m_Session.ActiveTrack = static_cast<int>(m_Session.Tracks.size()) - 1;
		}

		emit SessionChanged();
	}

	SubtitleTrack& Application::Track(int index)
	{
		return m_Session.Tracks[index];
	}

	const SubtitleTrack& Application::Track(int index) const
	{
		return m_Session.Tracks[index];
	}

	SubtitleTrack& Application::ActiveTrack()
	{
		return Track(m_Session.ActiveTrack);
	}

	const SubtitleTrack& Application::ActiveTrack() const
	{
		return Track(m_Session.ActiveTrack);
	}

	int Application::TrackCount() const
	{
		return static_cast<int>(m_Session.Tracks.size());
	}

	void Application::OpenSubtitle(const std::string& path)
	{
		m_Session.Tracks.clear();
		m_Session.ActiveTrack = -1;

		SubtitleTrack& track = CreateTrack();

		track = SRT::Load(path);
		track.Name = "Track 1";

		m_Session.SubtitlePath = path;

		m_Session.Tracks.push_back(m_Session.Tracks.front());

		emit SessionChanged();
	}

	void Application::OpenVideo(const std::string& path)
	{
		m_Session.VideoPath = path;
		m_Player.Load(path);

		emit SessionChanged();
	}

	void Application::SaveSubtitle(const std::string& path)
	{
		if (m_Session.Tracks.empty())
			return;

		if (path.empty())
		{
			// Save to Open SRT File Location
			SRT::Save(BuildExportTrack(), m_Session.SubtitlePath);
		}
		else
		{
			// Save to Path
			SRT::Save(BuildExportTrack(), path);
		}
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
		if (m_Session.Tracks.empty())
			return;

		int currentIndex = CurrentSubtitleIndex();

		if (currentIndex != -1)
		{
			Subtitle& current = ActiveTrack().Subtitles[currentIndex];

			if (m_Session.Playback.Position > current.Start)
			{
				m_Player.Seek(current.Start);
				return;
			}

			if (currentIndex > 0)
			{
				m_Player.Seek(ActiveTrack().Subtitles[currentIndex - 1].Start);
			}

			return;
		}

		int previousIndex = PreviousSubtitleIndex();

		if (previousIndex != -1)
		{
			m_Player.Seek(ActiveTrack().Subtitles[previousIndex].Start);
		}
	}

	void Application::NextSubtitleBoundary()
	{
		if (m_Session.Tracks.empty())
			return;

		int currentIndex = CurrentSubtitleIndex();

		if (currentIndex != -1)
		{
			Subtitle& current = ActiveTrack().Subtitles[currentIndex];

			if (m_Session.Playback.Position < current.End)
			{
				m_Player.Seek(current.End);
				return;
			}

			if (currentIndex + 1 < static_cast<int>(ActiveTrack().Subtitles.size()))
			{
				m_Player.Seek(ActiveTrack().Subtitles[currentIndex + 1].Start);
			}

			return;
		}

		int nextIndex = NextSubtitleIndex();

		if (nextIndex != -1)
		{
			m_Player.Seek(ActiveTrack().Subtitles[nextIndex].Start);
		}
	}

	Subtitle* Application::CurrentSubtitle()
	{
		if (m_Session.Tracks.empty())
			return nullptr;

		auto& subtitles = ActiveTrack().Subtitles;
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
		if (m_Session.Tracks.empty())
			return -1;

		const auto& subtitles = ActiveTrack().Subtitles;
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
		if (m_Session.Tracks.empty())
			return -1;

		const auto& subtitles = ActiveTrack().Subtitles;
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
		if (m_Session.Tracks.empty())
			return -1;

		const auto& subtitles = ActiveTrack().Subtitles;

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

	SubtitleTrack Application::BuildExportTrack()
	{
		SubtitleTrack output;
		output.Name = "Export";

		for (const auto& track : m_Session.Tracks)
		{
			if (!track.Visible)
				continue;

			output.Subtitles.insert(output.Subtitles.end(), track.Subtitles.begin(), track.Subtitles.end());
		}

		std::sort(output.Subtitles.begin(), output.Subtitles.end(), [](const Subtitle& lhs, const Subtitle& rhs) {
				return lhs.Start < rhs.Start;
			});

		return output;
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