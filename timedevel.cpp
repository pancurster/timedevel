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

Timedevel::Timedevel(TaskManager* tm, MainWindow* view, QObject* parent)
    : QObject(parent),
      m_taskManager(0), m_taskView(view)
{
    setTaskModel(tm);
}

Timedevel::~Timedevel()
{
}

void Timedevel::setTaskModel(TaskManager* manager)
{
    if (m_taskManager == manager)
        return;
    if (m_taskManager) {
        disconnect(m_taskManager, SIGNAL(taskAdded(Task*)),
                   m_taskView, SLOT(addTask(Task*)));
        disconnect(m_taskManager, SIGNAL(newActiveTask(Task*)),
                   m_taskView, SLOT(newActiveTask(Task*)));
        disconnect(m_taskManager, SIGNAL(taskElapsedTimeChanged(const QString&, int)),
                   m_taskView, SLOT(refreshElapsedTime(const QString&, int)));
    }

    m_taskManager = manager;
    if (m_taskManager) {
        connect(m_taskManager, SIGNAL(taskAdded(Task*)),
                m_taskView, SLOT(addTask(Task*)));
        connect(m_taskManager, SIGNAL(newActiveTask(Task*)),
                m_taskView, SLOT(newActiveTask(Task*)));
        connect(m_taskManager, SIGNAL(taskElapsedTimeChanged(const QString&, int)),
                m_taskView, SLOT(refreshElapsedTime(const QString&, int)));
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

    if (!m_taskManager->isEmpty()
        && m_taskManager->getActive()->getTaskName() == windowName) {
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
