#include "MainWindow.h"

#include <QMenuBar>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

namespace SubtitleStudio
{
    MainWindow::MainWindow(Application& studioApp, QWidget* parent)
        : m_StudioApp(studioApp), QMainWindow(parent)
    {
        resize(1280, 720);
        setWindowTitle("Subtitle Studio 0.4.9");

        CreateMenus();
        CreateCentralWidget();
        CreateDockWidgets();
    }

    void MainWindow::CreateMenus()
    {
        // --- File Menu ---
        QMenu* fileMenu = menuBar()->addMenu("&File");
        
        // Open Video
        m_OpenVideo = new QAction("Open Video...", this);
        connect(m_OpenVideo, &QAction::triggered, this, &MainWindow::OpenVideo);
        fileMenu->addAction(m_OpenVideo);
        
        // Open Subtitle
        m_OpenSubtitle = new QAction("Open Subtitle...", this);
        connect(m_OpenSubtitle, &QAction::triggered, this, &MainWindow::OpenSubtitle);
        fileMenu->addAction(m_OpenSubtitle);
        
        // Separator
        fileMenu->addSeparator();

        // Save
        m_Save = new QAction("Save", this);
        m_Save->setShortcut(QKeySequence::Save);
        connect(m_Save, &QAction::triggered, this, &MainWindow::SaveSubtitle);
        fileMenu->addAction(m_Save);
        
        // Save As
        m_SaveAs = new QAction("Save As...", this);
        m_SaveAs->setShortcut(QKeySequence::SaveAs);
        connect(m_SaveAs, &QAction::triggered, this, &MainWindow::SaveAsSubtitle);
        fileMenu->addAction(m_SaveAs);

        // Separator
        fileMenu->addSeparator();

        // Exit
        m_Exit = new QAction("Exit", this);
        connect(m_Exit, &QAction::triggered, this, []() {
                qDebug() << "Exit Clicked";
            });
        fileMenu->addAction(m_Exit);

        // --- View Menu ---
        QMenu* viewMenu = menuBar()->addMenu("&View");

        // Toggle Subtitle Properties
        m_ToggleSubtitleProperties = new QAction("Subtitle Properties Window", this);
        m_ToggleSubtitleProperties->setCheckable(true);
        connect(m_ToggleSubtitleProperties, &QAction::triggered, this, &MainWindow::ToggleSubtitleProperties);
        viewMenu->addAction(m_ToggleSubtitleProperties);
    }

    void MainWindow::CreateDockWidgets()
    {
        m_SubtitleDock = new QDockWidget("Subtitle Properties", this);
        m_SubtitleDock->setAllowedAreas(Qt::LeftDockWidgetArea |Qt::RightDockWidgetArea);

        m_SubtitleEditor = new SubtitleEditorWidget(m_SubtitleDock);
        m_SubtitleDock->setWidget(m_SubtitleEditor);
        addDockWidget(Qt::RightDockWidgetArea, m_SubtitleDock);

        connect(m_SubtitleEditor, &SubtitleEditorWidget::SubtitleChanged, this, [this]()
            {
                m_TimelineWidget->update();
            });

        m_SubtitleDock->hide();
        m_ToggleSubtitleProperties->setChecked(false);
    }

    void MainWindow::CreateCentralWidget()
    {
        auto* centralWidget = new QWidget(this);

        auto* layout = new QVBoxLayout(centralWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        m_VideoWidget = new VideoWidget(centralWidget);
        m_VideoWidget->SetApplication(&m_StudioApp);

        m_PlaybackControls = new PlaybackControlsWidget(centralWidget);
        m_PlaybackControls->SetApplication(&m_StudioApp);

        m_TimelineWidget = new TimelineWidget(centralWidget);
        m_TimelineWidget->SetApplication(&m_StudioApp);
        connect(m_TimelineWidget, &TimelineWidget::SubtitlePropertiesOpen, this, &MainWindow::OnSubtitlePropertiesOpen);

        layout->addWidget(m_VideoWidget);
        layout->addWidget(m_PlaybackControls);
        layout->addWidget(m_TimelineWidget);

        layout->setStretch(0, 3);
        layout->setStretch(1, 0);
        layout->setStretch(2, 2);

        setCentralWidget(centralWidget);
    }

    void MainWindow::OpenSubtitle()
    {
        QString filename = QFileDialog::getOpenFileName(this, "Open Subtitle", QString(), "SubRip (*.srt);;All Files (*)");

        if (filename.isEmpty())
        {
            return;
        }

        try
        {
            m_StudioApp.OpenSubtitle(filename.toStdString());
        }
        catch (const std::exception& e)
        {
            QMessageBox::critical(this, "Failed to Open Subtitle", e.what());
        }
    }

    void MainWindow::OpenVideo()
    {
        QString filename = QFileDialog::getOpenFileName(this, "Open Video", QString(),
            "Video Files (*.mp4 *.mkv *.avi *.mov *.wmv *.webm *.m4v);;All Files (*)");

        if (filename.isEmpty())
        {
            return;
        }

        try
        {
            m_StudioApp.OpenVideo(filename.toStdString());
        }
        catch (const std::exception& e)
        {
            QMessageBox::critical(this, "Failed to Open Video", e.what());
        }
    }

    void MainWindow::SaveSubtitle()
    {
        m_StudioApp.SaveSubtitle();
    }

    void MainWindow::SaveAsSubtitle()
    {
        QString filename = QFileDialog::getSaveFileName(this, "Save Subtitle As", QString(), "SubRip (*.srt);;All Files (*)");

        if (filename.isEmpty())
        {
            return;
        }

        try
        {
            m_StudioApp.SaveSubtitle(filename.toStdString());
        }
        catch (const std::exception& e)
        {
            QMessageBox::critical(this, "Failed to Save Subtitle As", e.what());
        }
    }

    void MainWindow::ToggleSubtitleProperties()
    {
        if (m_SubtitleDock->isHidden())
        {
            m_SubtitleDock->show();
            m_SubtitleDock->raise();
        }
        else
        {
            m_SubtitleDock->hide();
        }
    }

    void MainWindow::OnSubtitlePropertiesOpen(Subtitle* subtitle)
    {
        m_SubtitleEditor->SetSubtitle(subtitle);
        m_SubtitleDock->show();
        m_SubtitleDock->raise();
        m_SubtitleDock->activateWindow();
    }
}