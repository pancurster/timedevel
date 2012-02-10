/* main.cpp */
#include <memory>

#include <QxtGui/QxtApplication>

#include "mainwindow.h"
#include "focusdetector.h"
#include "timeractivator.h"
#include "taskmanager.h"
#include "timedevel.h"
#include "options_parsing.h"
#include "globals.h"

int main(int argc, char* argv[])
{
    std::auto_ptr<Options> local_options(new Options(argc, argv));
    if (local_options->parse())
        return 1;

    QxtApplication timedevelapp(argc, argv);
    FocusDetector* fdo = new FocusDetector;
    timedevelapp.installNativeEventFilter(fdo);

    // Singleton TaskManager
    TaskManager* tm = TaskManager::getInstance();

    // Widok
    MainWindow* mainWindow = new MainWindow;

    // controler-> setModel, setView
    Timedevel* taskControl = new Timedevel(tm, mainWindow, fdo, local_options);

    mainWindow->show();
    int ret = timedevelapp.exec();

    delete taskControl;
    delete tm;
    delete mainWindow;

    return ret; 
}
