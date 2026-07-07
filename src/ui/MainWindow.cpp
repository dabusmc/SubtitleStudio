#include "MainWindow.h"

#include <QMenuBar>
#include <QVBoxLayout>

namespace SubtitleStudio
{
    MainWindow::MainWindow(QWidget* parent)
        : QMainWindow(parent)
    {
        resize(1280, 720);
        setWindowTitle("Subtitle Studio 0.0.1");

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
        connect(m_OpenSubtitle, &QAction::triggered, this, []() {
                qDebug() << "Open Subtitle Clicked";
            });
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

        layout->addWidget(m_VideoWidget, 1);
        layout->addWidget(m_PlaybackControls);
        layout->addWidget(m_TimelineWidget);

        setCentralWidget(centralWidget);
    }
}