#pragma once

#include "ui/widgets/VideoWidget.h"
#include "ui/widgets/PlaybackControlsWidget.h"
#include "ui/widgets/TimelineWidget.h"

#include <QMainWindow>
#include <QAction>

namespace SubtitleStudio
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);

    private:
        void CreateMenus();
        void CreateCentralWidget();

    private:
        VideoWidget* m_VideoWidget = nullptr;
        PlaybackControlsWidget* m_PlaybackControls = nullptr;
        TimelineWidget* m_TimelineWidget = nullptr;

        QAction* m_OpenVideo = nullptr;
        QAction* m_OpenSubtitle = nullptr;
        QAction* m_Save = nullptr;
        QAction* m_SaveAs = nullptr;
        QAction* m_Exit = nullptr;
    };
}