/* main.cpp */
#include <QxtGui/QxtApplication>

#include "mainwindow.h"
#include "focusdetector.h"
#include "timeractivator.h"
#include "taskmanager.h"
#include "timedevel.h"

int main(int argc, char* argv[])
{
    QxtApplication timedevelapp(argc, argv);
    MainWindow* mainWindow = new MainWindow;

    // detektor focusa
    FocusDetector* fdo = new FocusDetector;
    timedevelapp.installNativeEventFilter(fdo);
    TimerActivator* t_activator = new TimerActivator(20);

    // Singleton TaskManager
    TaskManager* tm = TaskManager::getInstance();
    // controler-> setModel, setView
    Timedevel* taskControl = new Timedevel(tm, mainWindow, fdo);
    taskControl->addFocusActivator(fdo->signalHandle);
    taskControl->addFocusActivator(t_activator);

    mainWindow->show();
    int ret = timedevelapp.exec();

    delete taskControl;
    delete t_activator;
    delete tm;
    delete mainWindow;

    return ret; 
}
