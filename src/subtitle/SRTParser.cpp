#include "SRTParser.h"

#include <fstream>
#include <stdexcept>
#include <iostream>

#include "subtitle/Subtitle.h"
#include "util/Timecode.h"

#include <QList>
#include <QString>

namespace SubtitleStudio
{
	namespace SRT
	{
		// Source - https://stackoverflow.com/a/4654718
		// Posted by Charles Salvia, modified by community. See post 'Timeline' for change history
		// Retrieved 2026-07-07, License - CC BY-SA 4.0
		bool IsNumber(const std::string& str)
		{
			return !str.empty() && std::find_if(str.begin(), str.end(), [](unsigned char c) { return !std::isdigit(c); }) == str.end();
		}

		SubtitleTrack Load(const std::filesystem::path& path)
		{
			std::ifstream file(path);

			if (!file)
			{
				throw std::runtime_error("Failed to open subtitle file.");
			}

			SubtitleTrack track;

			Subtitle current = { };
			std::string line;
			while (std::getline(file, line))
			{
				if (line.empty())
				{
					continue;
				}

				if (IsNumber(line))
				{
					if (!current.Text.isEmpty())
					{
						track.Subtitles.push_back(current);
						current = { };
					}
					continue;
				}

				std::size_t pos = line.find(" --> ");

				if (pos != std::string::npos)
				{
					std::string startText = line.substr(0, pos);
					std::string endText = line.substr(pos + 5);

					current.Start = Timecode::FromSRT(QString::fromUtf8(startText.data(), startText.size()));
					current.End = Timecode::FromSRT(QString::fromUtf8(endText.data(), endText.size()));
				}
				else
				{
					if (current.Text.isEmpty())
					{
						current.Text = QString::fromUtf8(line.data(), line.size());
					}
					else
					{
						current.Text += '\n';
						current.Text += QString::fromUtf8(line.data(), line.size());
					}
				}
			}

			if (!current.Text.isEmpty())
			{
				track.Subtitles.push_back(current);
				current.Text = "";
			}

			return track;
		}

		void Save(const SubtitleTrack& track, const std::filesystem::path& path)
		{
			std::ofstream file(path, std::ios::binary);

			if (!file)
			{
				throw std::runtime_error("Failed to open subtitle file for writing.");
			}

			for (std::size_t i = 0; i < track.Subtitles.size(); ++i)
			{
				const Subtitle& s = track.Subtitles[i];

				// Index
				file << (i + 1) << "\n";

				// Timeline
				file << Timecode::ToSRT(s.Start).toStdString() << " --> " << Timecode::ToSRT(s.End).toStdString() << "\n";

				// Text (may contain multiple lines)
				auto parts = s.Text.split('\n');
				for (const auto& part : parts)
				{
					file << part.toUtf8().toStdString() << "\n";
				}

				// Blank line between entries
				file << "\n";
			}
		}
	}
}