//tasklistreader.cpp
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QString>
#include <QMap>
#include <QFile>

#include "task.h"
#include "tasklistreader.h"
#include "tasklistxmlhandler.h"

TaskListReader::TaskListReader(QMap<int, Task*>* tl, QString filename) :
    m_readDest(tl), m_filename(QString(filename))
{
}

int TaskListReader::read()
{
    QFile xmlFile(m_filename);
    QXmlInputSource source( &xmlFile);

    TaskListXmlHandler handler(m_readDest);

    QXmlSimpleReader reader;
    reader.setContentHandler( &handler);
    reader.setFeature("http://trolltech.com/xml/features/report-"
                      "whitespace-only-CharData", false);

    reader.parse( source);

    /* TODO: obsluga bledow! */
    return 0;
}

