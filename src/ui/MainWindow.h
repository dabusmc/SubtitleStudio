#pragma once

#include "app/Application.h"
#include "ui/widgets/VideoWidget.h"
#include "ui/widgets/PlaybackControlsWidget.h"
#include "ui/widgets/TimelineWidget.h"
#include "ui/widgets/SubtitleEditorWidget.h"

#include <QAction>
#include <QMainWindow>
#include <QDockWidget>

namespace SubtitleStudio
{
    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(Application& studioApp, QWidget* parent = nullptr);

    private:
        void CreateMenus();
        void CreateDockWidgets();
        void CreateCentralWidget();

    private:
        Application& m_StudioApp;

        QDockWidget* m_SubtitleDock = nullptr;
        SubtitleEditorWidget* m_SubtitleEditor = nullptr;
        VideoWidget* m_VideoWidget = nullptr;
        PlaybackControlsWidget* m_PlaybackControls = nullptr;
        TimelineWidget* m_TimelineWidget = nullptr;

        QAction* m_OpenVideo = nullptr;
        QAction* m_OpenSubtitle = nullptr;
        QAction* m_Save = nullptr;
        QAction* m_SaveAs = nullptr;
        QAction* m_Exit = nullptr;

    private slots:
        void OpenSubtitle();
        void OpenVideo();
        void OnSubtitlePropertiesOpen(Subtitle* subtitle);
    };
}