/* taskmanager.cpp */
#include <QDebug>

#include "taskmanager.h"
#include "tasklistwriter.h"
#include "tasklistreader.h"
#include "windowattr.h"

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
    if ( ! t->hasParent()) {
        trySetParent(t);
    }
    insert(key, t);
    qDebug() <<"Dodano zadanie: "<<t->getTaskName();
    emit taskAdded(t);
}

void TaskManager::add(const QString& key)
{
    Task* t = new Task(key);
    if (t) {
        insert(key, t);
        qDebug() <<"Dodano zadanie: "<<t->getTaskName();
        emit taskAdded(t);
    }
}

void TaskManager::trySetParent(Task* t)
{
    QString app_name = t->getAppName();
    QMap<QString, Task*>::iterator i = this->begin();
    while (i != this->end()) {
        if (i.value()->getAppName() == app_name && !i.value()->hasParent()) {
            t->setParent(i.value());

            qDebug()<<"Zadanie: "<<t->getTaskName()<<" ma rodzica: "
                    <<i.value()->getTaskName();
            break; // Sukces! Dalej nie szukamy.
        }
        ++i;
    }
    /*
     * Moze lepszym rozwiazeniem jest zrobienie mapy z nazwa programu
     * jako kluczem?
     */
}

void TaskManager::remove(const QString& taskName)
{
    if (contains(taskName)) {
        //TODO nie mam pojecia dlaczego remove nie dziala
        take(taskName);
        emit taskRemoved(taskName);
    }
}

void TaskManager::setName(const QString& taskName, const QString& newName)
{
    qDebug() << "SETNAME";
    Task* t = value(taskName);
    if (!t)
        qDebug() << "   BLAD: "<< Q_FUNC_INFO<< ", nie znaleziono zadania";
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
            if (m_activeTask->hasParent()) {
                emit taskElapsedTimeChanged(
                        m_activeTask->getParent()->getTaskName(),
                        m_activeTask->getParent()->getElpasedChildrenTime()
                        + m_activeTask->getParent()->getElapsedTime());
            }
        }
        m_activeTask = value(taskName);
        m_activeTask->startTimer();
        emit newActiveTask(m_activeTask);

    } else {
        qDebug() << "   BLAD!," << Q_FUNC_INFO<< " : brak zadania"
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
    this->clear();
    if ( !taskReader.read() )
        emit readDone(true);
}

