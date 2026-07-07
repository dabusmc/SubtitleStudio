#include <filesystem>
#include <iostream>

#include "app/Application.h"
#include "ui/MainWindow.h"
#include "subtitle/SRTParser.h"

#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    SubtitleStudio::Application studioApp;

    SubtitleStudio::MainWindow window(studioApp);
    window.show();

    return app.exec();
}