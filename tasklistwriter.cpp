// tasklistwriter.cpp
#include <QDebug>
#include <QXmlStreamWriter>
#include <QString>
#include <QFile>
#include <QMap>

#include "tasklistwriter.h"
#include "task.h"

TaskListWriter::TaskListWriter(QMap<QString, Task*>* tasksToWrite,
                               const QString filename)
    : m_tToWrite(tasksToWrite), m_filename(filename)
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
    stream->writeStartElement("task_list");
 
    QMap<QString, Task*>::iterator i = m_tToWrite->begin();
    while (i != m_tToWrite->end()) {
        stream->writeStartElement("task");
        stream->writeTextElement("name", i.value()->getTaskName());
        //stream->writeAttribute("type", "TO_DO");
        stream->writeTextElement("time_elapsed", 
                                 i.value()->getElapsedTimeString());
        stream->writeEndElement();
        ++i;
    }
    stream->writeEndElement(); //task_list
    stream->writeEndDocument();

    /* IN Version 4.8 Qt
    if (stream->hasError())
        return 1;
    */
    qDebug() << "Zakonczono dzialanie tasklistwriter::write sukcesem";
    return 0;
}
