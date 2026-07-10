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
        setWindowTitle("Subtitle Studio 0.0.4");

        CreateMenus();
        CreateCentralWidget();
    }

    void MainWindow::CreateMenus()
    {
        QMenu* fileMenu = menuBar()->addMenu("&File");
        
        m_OpenVideo = new QAction("Open Video...", this);
        connect(m_OpenVideo, &QAction::triggered, this, []() {
                qDebug() << "Open Video Clicked";
            });
        
        fileMenu->addAction(m_OpenVideo);
        
        m_OpenSubtitle = new QAction("Open Subtitle...", this);
        connect(m_OpenSubtitle, &QAction::triggered, this, &MainWindow::OpenSubtitle);
        fileMenu->addAction(m_OpenSubtitle);
        
        fileMenu->addSeparator();

        m_Save = new QAction("Save", this);
        m_Save->setShortcut(QKeySequence::Save);
        connect(m_Save, &QAction::triggered, this, []() {
                qDebug() << "Save Subtitles Clicked";
            });
        fileMenu->addAction(m_Save);
        
        m_SaveAs = new QAction("Save As...", this);
        m_SaveAs->setShortcut(QKeySequence::SaveAs);
        connect(m_SaveAs, &QAction::triggered, this, []() {
                qDebug() << "Save Subtitles As Clicked";
            });
        fileMenu->addAction(m_SaveAs);

        fileMenu->addSeparator();

        m_Exit = new QAction("Exit", this);
        connect(m_Exit, &QAction::triggered, this, []() {
                qDebug() << "Exit Clicked";
            });
        fileMenu->addAction(m_Exit);
    }

    void MainWindow::CreateCentralWidget()
    {
        auto* centralWidget = new QWidget(this);

        auto* layout = new QVBoxLayout(centralWidget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        m_VideoWidget = new VideoWidget(centralWidget);
        m_PlaybackControls = new PlaybackControlsWidget(centralWidget);
        m_TimelineWidget = new TimelineWidget(centralWidget);
        m_TimelineWidget->SetApplication(&m_StudioApp);

        layout->addWidget(m_VideoWidget, 1);
        layout->addWidget(m_PlaybackControls);
        layout->addWidget(m_TimelineWidget);

        setCentralWidget(centralWidget);
    }

    void MainWindow::OpenSubtitle()
    {
        QString filename = QFileDialog::getOpenFileName(
            this,
            "Open Subtitle",
            QString(),
            "SubRip (*.srt);;All Files (*)"
        );

        // User pressed Cancel.
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
            QMessageBox::critical(
                this,
                "Failed to Open Subtitle",
                e.what());
        }
    }
}