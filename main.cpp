/* main.cpp */
#include <QxtGui/QxtApplication>

#include "mainwindow.h"
#include "focusdetector.h"
#include "taskmanager.h"

int main(int argc, char* argv[])
{
    QxtApplication timedevelapp(argc, argv);
    MainWindow* mainWindow = new MainWindow;

    /* controler->setModel() */
    TaskManager* tm = TaskManager::getInstance();
    mainWindow->setTaskModelManager(tm);

    /* detektor focusa */
    FocusDetector* fdo = new FocusDetector(mainWindow); 
    timedevelapp.installNativeEventFilter(fdo);

    mainWindow->show();
    return timedevelapp.exec();
}
