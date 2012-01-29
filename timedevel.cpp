/* timedevel.cpp */
#include <QDebug>
#include <QWidget>
#include <QString>

#include <QxtGui/QxtWindowSystem>

#include "timedevel.h"
#include "windowattr.h"
#include "task.h"
#include "mainwindow.h"
#include "taskmanager.h"
#include "focusdetector.h"

Timedevel::Timedevel(TaskManager* tm, MainWindow* view,
                     FocusDetector* fdo, QObject* parent):
    QObject(parent),
    m_taskManager(0),
    m_taskView(view),
    m_focusDetector(fdo)
{
    setTaskModel(tm);
    m_taskManager->readFromFile();

}

Timedevel::~Timedevel()
{
    m_taskManager->writeToFile();
}

void Timedevel::setTaskModel(TaskManager* manager)
{
    if (m_taskManager == manager)
        return;
    if (m_taskManager) {
        // Wejscie z FocusDetector
        disconnect(m_focusDetector->signalHandle, SIGNAL(focusChanged()),
                   this, SLOT(processFocusChange()));

        // Wejscie z Modelu 
        disconnect(m_taskManager, SIGNAL(taskAdded(Task*)),
                   m_taskView, SLOT(addTask(Task*)));
        disconnect(m_taskManager, SIGNAL(newActiveTask(Task*)),
                   m_taskView, SLOT(newActiveTask(Task*)));
        disconnect(m_taskManager, SIGNAL(taskElapsedTimeChanged(const QString&, int)),
                   m_taskView, SLOT(refreshElapsedTime(const QString&, int)));

        // Wejscie z Widoku
        disconnect(m_taskView, SIGNAL(orderQuit()), qApp, SLOT(quit()));
        disconnect(m_taskView, SIGNAL(orderLoad()),
                m_taskManager, SLOT(readFromFile()));
        disconnect(m_taskView, SIGNAL(orderSave()),
                m_taskManager, SLOT(writeToFile()));
        disconnect(m_taskView, SIGNAL(orderEditTaskName(const QString&,const QString&)),
                m_taskManager, SLOT(setName(const QString&, const QString&)));
    }

    m_taskManager = manager;
    if (m_taskManager) {
        // Wejscie z FocusDetector
        connect(m_focusDetector->signalHandle, SIGNAL(focusChanged()),
                this, SLOT(processFocusChange()));

        // Wejscie z Modelu
        // przedrostek announce przy syg. wychodzacych z modelu ?
        connect(m_taskManager, SIGNAL(taskAdded(Task*)),
                m_taskView, SLOT(addTask(Task*)));
        connect(m_taskManager, SIGNAL(newActiveTask(Task*)),
                m_taskView, SLOT(newActiveTask(Task*)));
        connect(m_taskManager, SIGNAL(taskElapsedTimeChanged(const QString&, int)),
                m_taskView, SLOT(refreshElapsedTime(const QString&, int)));

        // Wejscie z Widoku
        connect(m_taskView, SIGNAL(orderQuit()), qApp, SLOT(quit()));
        connect(m_taskView, SIGNAL(orderLoad()),
                m_taskManager, SLOT(readFromFile()));
        connect(m_taskView, SIGNAL(orderSave()),
                m_taskManager, SLOT(writeToFile()));
        connect(m_taskView, SIGNAL(orderEditTaskName(const QString&,const QString&)),
                m_taskManager, SLOT(setName(const QString&, const QString&)));
    }
}

void Timedevel::setTaskView(MainWindow* taskviewer)
{
    m_taskView= taskviewer;
}

void Timedevel::processFocusChange()
{
    // Narazie kiepsko dziala
    //int WId = getActiveWId();

    WindowAttr* wa = new WindowAttr;
    QString windowName = wa->getWName();
    Task* t = 0;

    if (!m_taskManager->isEmpty() &&
        m_taskManager->getActive() != 0 &&
        m_taskManager->getActive()->getTaskName() == windowName) {
        return;
    } else {
        if (m_taskManager->contains(windowName)) {
            m_taskManager->setActiveTask(windowName);
        } else {
            t = new Task(wa);
            m_taskManager->add(windowName, t);
            m_taskManager->setActiveTask(windowName);
            return;
        }
    }
    delete wa;
    qDebug() << "Focus na oknie: " << windowName;
}

int Timedevel::getActiveWId()
{
    return QxtWindowSystem::activeWindow();
}

