#pragma once

#include "Session.h"
#include "video/VideoPlayer.h"
#include "subtitle/editor/SubtitleEditor.h"

#include <QObject>

namespace SubtitleStudio
{
	class Application : public QObject
	{
		Q_OBJECT

	public:
		explicit Application(QObject* parent = nullptr);

		Session& GetSession() { return m_Session; }
		VideoPlayer& GetVideoPlayer() { return m_Player; }
		SubtitleEditor& GetSubtitleEditor() { return m_Editor; }

		bool TracksAvailable() const;
		SubtitleTrack& CreateTrack();

		void RemoveTrack(int index);

		SubtitleTrack& Track(int index);
		const SubtitleTrack& Track(int index) const;

		SubtitleTrack& ActiveTrack();
		const SubtitleTrack& ActiveTrack() const;

		int TrackCount() const;

		void OpenSubtitle(const std::string& path);
		void OpenVideo(const std::string& path);

		void SaveSubtitle(const std::string& path = "");

		void PlayPause();

		void SeekRelative(std::chrono::milliseconds offset);

		void PreviousSubtitleBoundary();
		void NextSubtitleBoundary();

		Subtitle* CurrentSubtitle();

	private:
		int CurrentSubtitleIndex() const;
		int PreviousSubtitleIndex() const;
		int NextSubtitleIndex() const;

		int SubtitleIndexAt(std::chrono::milliseconds position) const;

		SubtitleTrack BuildExportTrack();

	signals:
		void SessionChanged();
		void PlayingStateChanged(bool playing);
		void PlaybackPositionChanged(std::chrono::milliseconds position);

	private slots:
		void OnVideoLoaded(std::chrono::milliseconds duration);
		void OnPositionChanged(std::chrono::milliseconds position);
		void OnPlayingStateChanged(bool playing);

	private:
		Session m_Session;
		SubtitleEditor m_Editor;
		VideoPlayer m_Player;
	};
}