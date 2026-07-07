#pragma once

#include "Session.h"

#include <QObject>

namespace SubtitleStudio
{
	class Application : public QObject
	{
		Q_OBJECT

	public:
		explicit Application(QObject* parent = nullptr);

		Session& GetSession();

		void OpenSubtitle(const std::string& path);

	signals:
		void SessionChanged();

	private:
		Session m_Session;
	};
}