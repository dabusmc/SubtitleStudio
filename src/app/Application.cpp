#include "Application.h"

#include <iostream>

#include "subtitle/SRTParser.h"

namespace SubtitleStudio
{
	Application::Application(QObject* parent)
		: QObject(parent), m_Session()
	{
	}

	Session& Application::GetSession()
	{
		return m_Session;
	}

	void Application::OpenSubtitle(const std::string& path)
	{
		m_Session.Track = SRT::Load(path);
		m_Session.SubtitlePath = path;

		std::cout << m_Session.Track << std::endl;

		emit SessionChanged();
	}
}