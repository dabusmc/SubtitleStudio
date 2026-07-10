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

	signals:
		void SessionChanged();
		void PlaybackPositionChanged(std::chrono::milliseconds position);

	private slots:
		void OnPositionChanged(std::chrono::milliseconds position);

	private:
		Session m_Session;
		VideoPlayer m_Player;
	};
}