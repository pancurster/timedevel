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

void Timedevel::addFocusActivator(FocusActivator* f_activator)
{
    connect(f_activator, SIGNAL(focusChanged()),
            this, SLOT(processFocusChange()));
}

void Timedevel::removeFocusActivator(FocusActivator* f_activator)
{
    disconnect(f_activator, SIGNAL(focusChanged()),
               this, SLOT(processFocusChange()));
}

void Timedevel::setTaskModel(TaskManager* manager)
{
    if (m_taskManager == manager)
        return;
    if (m_taskManager) {
        // Wejscie z Modelu 
        disconnect(m_taskManager, SIGNAL(taskAdded(Task*)),
                   m_taskView, SLOT(addTask(Task*)));
        disconnect(m_taskManager, SIGNAL(newActiveTask(Task*)),
                   m_taskView, SLOT(newActiveTask(Task*)));
        disconnect(m_taskManager, SIGNAL(taskElapsedTimeChanged(const QString&, int)),
                   m_taskView, SLOT(refreshElapsedTime(const QString&, int)));
        disconnect(m_taskManager, SIGNAL(taskRemoved(const QString&)),
                   m_taskView, SLOT(removeTaskFromView(const QString&)));

        // Wejscie z Widoku
        disconnect(m_taskView, SIGNAL(orderQuit()), qApp, SLOT(quit()));
        disconnect(m_taskView, SIGNAL(orderLoad()),
                m_taskManager, SLOT(readFromFile()));
        disconnect(m_taskView, SIGNAL(orderSave()),
                m_taskManager, SLOT(writeToFile()));
        disconnect(m_taskView, SIGNAL(orderEditTaskName(const QString&,const QString&)),
                m_taskManager, SLOT(setName(const QString&, const QString&)));
        disconnect(m_taskView, SIGNAL(orderRemoveTask(const QString&)),
                m_taskManager, SLOT(remove(const QString&)));
        disconnect(m_taskView, SIGNAL(orderNewTask(const QString&)),
                m_taskManager, SLOT(add(const QString&)));
        disconnect(m_taskView, SIGNAL(offFocusDetector()),
                this, SLOT(offFocusDetector()));
        disconnect(m_taskView, SIGNAL(onFocusDetector()),
                this, SLOT(onFocusDetector()));
    }

    m_taskManager = manager;
    if (m_taskManager) {
        // Wejscie z Modelu
        // przedrostek announce przy syg. wychodzacych z modelu ?
        connect(m_taskManager, SIGNAL(taskAdded(Task*)),
                m_taskView, SLOT(addTask(Task*)));
        connect(m_taskManager, SIGNAL(newActiveTask(Task*)),
                m_taskView, SLOT(newActiveTask(Task*)));
        connect(m_taskManager, SIGNAL(taskElapsedTimeChanged(const QString&, int)),
                m_taskView, SLOT(refreshElapsedTime(const QString&, int)));
        connect(m_taskManager, SIGNAL(taskRemoved(const QString&)),
                m_taskView, SLOT(removeTaskFromView(const QString&)));

        // Wejscie z Widoku
        connect(m_taskView, SIGNAL(orderQuit()), qApp, SLOT(quit()));
        connect(m_taskView, SIGNAL(orderLoad()),
                m_taskManager, SLOT(readFromFile()));
        connect(m_taskView, SIGNAL(orderSave()),
                m_taskManager, SLOT(writeToFile()));
        connect(m_taskView, SIGNAL(orderEditTaskName(const QString&,const QString&)),
                m_taskManager, SLOT(setName(const QString&, const QString&)));
        connect(m_taskView, SIGNAL(orderRemoveTask(const QString&)),
                m_taskManager, SLOT(remove(const QString&)));
        connect(m_taskView, SIGNAL(orderNewTask(const QString&)),
                m_taskManager, SLOT(add(const QString&)));
        connect(m_taskView, SIGNAL(offFocusDetector()),
                this, SLOT(offFocusDetector()));
        connect(m_taskView, SIGNAL(onFocusDetector()),
                this, SLOT(onFocusDetector()));
    }
}

void Timedevel::offFocusDetector()
{
    qxtApp->removeNativeEventFilter(m_focusDetector);
}

void Timedevel::onFocusDetector()
{
    qxtApp->installNativeEventFilter(m_focusDetector);
}

void Timedevel::setTaskView(MainWindow* taskviewer)
{
    m_taskView = taskviewer;
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

