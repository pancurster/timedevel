//tasklistreader.cpp
#include <QXmlSimpleReader>
#include <QXmlInputSource>
#include <QString>
#include <QMap>
#include <QFile>

#include "task.h"
#include "taskmanager.h"
#include "tasklistreader.h"
#include "tasklistxmlhandler.h"

TaskListReader::TaskListReader(TaskManager* tm, QString filename) :
    m_readDest(tm), m_filename(QString(filename))
{
}

int TaskListReader::read()
{
    TaskListXmlHandler handler(m_readDest);

    QXmlSimpleReader reader;
    reader.setContentHandler( &handler );
    // Ignoruje znaki spacji, tabulacji itd.
    reader.setFeature("http://trolltech.com/xml/features/report-"
                      "whitespace-only-CharData", 
                      false);

    QFile xmlFile(m_filename);
    QXmlInputSource source( &xmlFile);
    reader.parse(source);

    /* TODO: obsluga bledow! */
    return 0;
}

