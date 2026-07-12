#include "SubtitleEditor.h"

namespace SubtitleStudio
{
	SubtitleEditor::SubtitleEditor(Session& session)
		: m_Session(session)
	{
	}

	void SubtitleEditor::BeginDrag(const SubtitleSelection& selection, QPoint mouseStart)
	{
		m_DragState.Mode = DragMode::Move;
		m_DragState.MouseStart = mouseStart;
		m_DragState.Selection = selection;
		m_DragState.OriginalSubtitle = *selection.Subtitle;
		m_DragState.TargetTrack = selection.TrackIndex;
	}

	void SubtitleEditor::DragBy(std::chrono::milliseconds delta, int targetTrack)
	{
		if (!m_DragState.Selection.Subtitle)
			return;

		if (m_DragState.OriginalSubtitle.Start + delta < std::chrono::milliseconds(0))
		{
			delta = -m_DragState.OriginalSubtitle.Start;
		}

		Subtitle* subtitle = m_DragState.Selection.Subtitle;
		subtitle->Start = m_DragState.OriginalSubtitle.Start + delta;
		subtitle->End = m_DragState.OriginalSubtitle.End + delta;

		m_DragState.TargetTrack = targetTrack;
	}

	void SubtitleEditor::EndDrag()
	{
		if (!m_DragState.Selection.Subtitle)
		{
			m_DragState = { };
			return;
		}

		int sourceTrack = m_DragState.Selection.TrackIndex;
		int destinationTrack = m_DragState.TargetTrack;

		if (destinationTrack == static_cast<int>(m_Session.Tracks.size()))
		{
			CreateTrack();
		}

		if (sourceTrack != destinationTrack)
		{
			auto& source = m_Session.Tracks[sourceTrack].Subtitles;
			auto& destination = m_Session.Tracks[destinationTrack].Subtitles;

			auto it = std::find_if(source.begin(), source.end(), [this](const auto& subtitle) {
					return subtitle.get() == m_DragState.Selection.Subtitle;
				});

			if (it == source.end())
			{
				m_DragState = { };
				return;
			}

			destination.push_back(std::move(*it));
			source.erase(it);

			std::sort(destination.begin(), destination.end(), [](const auto& lhs, const auto& rhs) {
					return lhs->Start < rhs->Start;
				});
		}
		else
		{
			auto& subtitles = m_Session.Tracks[sourceTrack].Subtitles;
			std::sort(subtitles.begin(), subtitles.end(), [](const auto& lhs, const auto& rhs){
					return lhs->Start < rhs->Start;
				});
		}

		m_DragState = { };
	}

	SubtitleTrack& SubtitleEditor::CreateTrack()
	{
		SubtitleTrack& track = m_Session.Tracks.emplace_back();
		track.Name = "Track " + std::to_string(m_Session.Tracks.size());
		return track;
	}
}