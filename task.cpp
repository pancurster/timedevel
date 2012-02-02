/* task.cpp */
#include <QTime>
#include <QString>
#include <QList>

#include "task.h"
#include "windowattr.h"

Task::Task(WindowAttr* wa):
    /* W tym przypadku nazwa zadania to nazwa okna */
    m_taskName(wa->getWName()),
    m_appName(wa->getAppName()),
    m_elapsed(0),

    /* Zero jesli to jest top level task */
    m_parentTask(0),
    m_childrenTask(new QList<Task*>)
{
    m_wattr = wa;
    m_time = new QTime(0,0,0,0);
    m_time->start();
}

Task::Task(const QString taskName):
    m_taskName(taskName),
    m_appName(""),
    m_elapsed(0),
    m_wattr(0),
    m_parentTask(0),
    m_childrenTask(new QList<Task*>)
{
    m_time = new QTime(0,0,0,0);
}

Task::Task(const QString taskName, const int t_elapsed):
    m_taskName(taskName),
    m_appName(""),
    m_elapsed(t_elapsed),
    m_wattr(0),
    m_parentTask(0),
    m_childrenTask(new QList<Task*>)
{
    m_time = new QTime(0,0,0,0);
}

Task::~Task()
{
    delete m_wattr;
    delete m_time;
    delete m_childrenTask;
}

QString Task::getAppName()
{
    return m_appName;
}

const WindowAttr* Task::getWAttr()
{
    return m_wattr;
}

QString Task::getTaskName()
{
    return m_taskName;
}

int Task::getElapsedTime()
{
    return m_elapsed;
}

QString Task::getElapsedTimeString()
{
    return QString::number(m_elapsed);
}

void Task::startTimer()
{
    m_time->restart();
}

void Task::stopTimer()
{
    m_elapsed += m_time->elapsed();
}

bool Task::hasWAttr()
{
    return m_wattr ? true : false;
}

void Task::setAppName(const QString& appName)
{
    m_appName = appName;
}

void Task::setName(const QString& taskName)
{
    m_taskName = taskName;
}

void Task::setParent(Task* t)
{
    // Jesli zadanie mialo rodzica to rodzic usuwa dziecko
    if (m_parentTask) {
        m_parentTask->removeChildTask(this);
    }

    // Ustawia nowego rodzica
    m_parentTask = t;

    // Informuje nowego rodzica o nowym dziecku
    // Jesli t==0 to zadanie staje sie topLevelTask
    if (t)
        t->addChildTask(this);
}

Task* Task::getParent()
{
    return m_parentTask ? m_parentTask : this;
}

bool Task::hasParent()
{
    return m_parentTask ? true : false;
}

void Task::setElapsed(int elapsed)
{
    m_elapsed = elapsed;
}

void Task::addChildTask(Task* t)
{
    m_childrenTask->append(t);
}

void Task::removeChildTask(Task* t)
{
    m_childrenTask->removeOne(t);
}

QList<Task*> Task::getChildren()
{
    return *m_childrenTask;
}

int Task::getElpasedChildrenTime()
{
    if (m_childrenTask->isEmpty())
        return 0;

    int sumTime = 0;
    foreach(Task* t, *m_childrenTask) {
        sumTime += t->getElapsedTime();
    }

    return sumTime;
}

