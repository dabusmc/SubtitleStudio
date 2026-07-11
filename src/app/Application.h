#pragma once

#include "Session.h"
#include "video/VideoPlayer.h"

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
		VideoPlayer m_Player;
	};
}