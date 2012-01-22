/* main.cpp */
#include <QxtGui/QxtApplication>

#include "mainwindow.h"
#include "focusdetector.h"
#include "taskmanager.h"
#include "timedevel.h"

int main(int argc, char* argv[])
{
    QxtApplication timedevelapp(argc, argv);
    MainWindow* mainWindow = new MainWindow;

    /* detektor focusa */
    FocusDetector* fdo = new FocusDetector;
    timedevelapp.installNativeEventFilter(fdo);

    /* Singleton TaskManager */
    TaskManager* tm = TaskManager::getInstance();
    /* controler-> setModel, setView */
    Timedevel* taskControl = new Timedevel(tm, mainWindow, fdo);

    mainWindow->show();
    return timedevelapp.exec();
}
