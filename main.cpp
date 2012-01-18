/* main.cpp */
#include <QxtGui/QxtApplication>

#include "mainwindow.h"
#include "focusdetector.h"

int main(int argc, char* argv[])
{
    QxtApplication timedevelapp(argc, argv);

    MainWindow* mainWindow = new MainWindow;

    FocusDetector* fdo = new FocusDetector(mainWindow); 
    timedevelapp.installNativeEventFilter(fdo);

    mainWindow->show();
    return timedevelapp.exec();
}
