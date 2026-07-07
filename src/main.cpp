#include <filesystem>
#include <iostream>

#include <QApplication>

#include "ui/MainWindow.h"
#include "subtitle/SRTParser.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    SubtitleStudio::MainWindow window;
    window.show();

    return app.exec();
}