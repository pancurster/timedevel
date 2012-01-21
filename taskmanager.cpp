/* taskmanager.cpp */
#include <QDebug>

#include "taskmanager.h"
#include "tasklistwriter.h"
#include "tasklistreader.h"

TaskManager* TaskManager::theTaskManager = 0;

TaskManager::TaskManager(QObject* parent)
    : QObject(parent), QMap<QString, Task*>(),
    m_activeTask(0)
{
}

TaskManager::~TaskManager()
{
    if (theTaskManager == this)
        theTaskManager = 0;
}

TaskManager* TaskManager::getInstance()
{
    if (!theTaskManager)
        theTaskManager = new TaskManager;
    return theTaskManager;
}

void TaskManager::add(const QString& key, Task* t)
{
    insert(key, t);
    emit taskAdded(t);
}

void TaskManager::remove(const QString& taskName)
{
    remove(taskName);
    emit taskRemoved(taskName);
}

void TaskManager::setName(const QString& taskName, const QString& newName)
{
    Task* t = value(taskName);
    t->setName(newName);
    emit taskChanged(t);
}

void TaskManager::setElapsed(const QString& taskName, const int elapsed)
{
    Task* t = value(taskName);
    t->setElapsed(elapsed);
    emit taskChanged(t);
}

void TaskManager::setActiveTask(const QString& taskName)
{
    if (!isEmpty() && contains(taskName)) {
        if (m_activeTask) {
            m_activeTask->stopTimer();
            emit taskElapsedTimeChanged(m_activeTask->getTaskName(),
                                        m_activeTask->getElapsedTime());
        }

        m_activeTask = value(taskName);
        m_activeTask->startTimer();
        emit newActiveTask(m_activeTask);
    } else {
        qDebug() << "BLAD!, TaskManager::setActiveTask: brak zadania"
                 << taskName;
    }
}

Task* TaskManager::getActive()
{
    return m_activeTask;
}

/* TODO Dodac obsluge bledow. Wtedy parametr writeDone bedzie mial sens */
void TaskManager::writeToFile()
{
    TaskListWriter taskWriter(this, "tasklist.xml");
    if ( !taskWriter.write() )
        emit writeDone(true);
}

void TaskManager::readFromFile()
{
    TaskListReader taskReader(this, "tasklist.xml");
    if ( !taskReader.read() )
        emit readDone(true);
}

