/* task.cpp */
#include <QTime>
#include <QString>
#include <QList>

#include "task.h"
#include "windowattr.h"

Task::Task(WindowAttr* wa):
    /* W tym przypadku nazwa zadania to nazwa okna */
    m_taskName(wa->getWName()),
    m_elapsed(0),

    /* Zero jesli to jest top level task */
    m_parentTask(0)
{
    m_wattr = wa;
    m_time = new QTime(0,0,0,0);
    m_time->start();
}

Task::Task(const QString taskName):
    m_taskName(taskName),
    m_elapsed(0),
    m_wattr(0),
    m_parentTask(0)
{
    m_time = new QTime(0,0,0,0);
}

Task::Task(const QString taskName, const int t_elapsed):
    m_taskName(taskName),
    m_elapsed(t_elapsed),
    m_wattr(0),
    m_parentTask(0)
{
    m_time = new QTime(0,0,0,0);
}

Task::~Task()
{
    delete m_wattr;
    delete m_time;
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
    //m_elapsed += m_time->elapsed();
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
    if (m_wattr)
        return true;
    else
        return false;
}

void Task::setName(const QString& taskName)
{
    m_taskName = taskName;
}

void Task::setElapsed(int elapsed)
{
    m_elapsed = elapsed;
}
