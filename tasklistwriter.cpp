// tasklistwriter.cpp
#include <QDebug>
#include <QXmlStreamWriter>
#include <QString>
#include <QFile>
#include <QMap>

#include "tasklistwriter.h"
#include "task.h"

TaskListWriter::TaskListWriter(QMap<QString, Task*>* taskToWrite,
                               const QString filename)
    : m_tToWrite(taskToWrite), m_filename(filename)
{
}

int TaskListWriter::write()
{
    QFile file(m_filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return 1;

    stream = new QXmlStreamWriter(&file);

    if (m_tToWrite->empty())
        return 1;

    stream->setAutoFormatting(true);
    stream->writeStartDocument();
    stream->writeStartElement("timedevel_task_list");

    stream->writeStartElement("top_task");
    QMap<QString, Task*>::iterator i = m_tToWrite->begin();
    while (i != m_tToWrite->end()) {
        if (i.value()->hasParent()) {
            ++i;
            continue;
        }
        stream->writeStartElement("task");
        stream->writeTextElement("name", i.value()->getTaskName());
        stream->writeTextElement("app_name", i.value()->getAppName());
        stream->writeTextElement("parent", "None"); 
        stream->writeTextElement("time_elapsed", 
                                 i.value()->getElapsedTimeString());
        stream->writeEndElement();
        ++i;
    }
    stream->writeEndElement();

    stream->writeStartElement("child_task");
    i = m_tToWrite->begin();
    while (i != m_tToWrite->end()) {
        if (! i.value()->hasParent()) {
            ++i;
            continue;
        }
        stream->writeStartElement("task");
        stream->writeTextElement("name", i.value()->getTaskName());
        stream->writeTextElement("app_name", i.value()->getAppName());
        stream->writeTextElement("parent", i.value()->getParent()->getTaskName());
        stream->writeTextElement("time_elapsed", 
                                 i.value()->getElapsedTimeString());
        stream->writeEndElement();
        ++i;
    }
    stream->writeEndElement();

    stream->writeEndElement();
    stream->writeEndDocument();

    /* IN Version 4.8 Qt
    if (stream->hasError())
        return 1;
    */
    qDebug() << "Zakonczono dzialanie"<< Q_FUNC_INFO << "sukcesem";
    return 0;
}

