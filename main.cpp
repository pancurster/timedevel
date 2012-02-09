/* main.cpp */
#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include <QxtGui/QxtApplication>

#include "mainwindow.h"
#include "focusdetector.h"
#include "timeractivator.h"
#include "taskmanager.h"
#include "timedevel.h"

int options(int argc, char* argv[], Timedevel* app)
{
    namespace po = boost::program_options;

    po::options_description desc("Timedevel 0.1 ver, allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("taskfile", po::value<std::string>(),
         "set file to read & write tracked tasks")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    if (vm.count("taskfile")) {
        app->setTaskFile(vm["taskfile"].as< std::string >());
    }
    return 0;
}

int main(int argc, char* argv[])
{
    QxtApplication timedevelapp(argc, argv);
    FocusDetector* fdo = new FocusDetector;
    timedevelapp.installNativeEventFilter(fdo);

    // Singleton TaskManager
    TaskManager* tm = TaskManager::getInstance();
    // Widok
    MainWindow* mainWindow = new MainWindow;
    // controler-> setModel, setView
    Timedevel* taskControl = new Timedevel(tm, mainWindow, fdo);
    int ret = options(argc, argv, taskControl);
    if (ret)
        return 1;
    taskControl->readTaskFile();

    mainWindow->show();
    ret = timedevelapp.exec();

    delete taskControl;
    delete tm;
    delete mainWindow;

    return ret; 
}
