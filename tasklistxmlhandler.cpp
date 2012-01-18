//tasklistxmlhandler.cpp
#include <QDebug>
#include <QMap>
//#include <QString>
#include <QXmlAttributes>
#include <QXmlParseException>

#include "task.h"
#include "tasklistxmlhandler.h"

const QString TaskListXmlHandler::TOKEN_task_list ("task_list");
const QString TaskListXmlHandler::TOKEN_task      ("task");
const QString TaskListXmlHandler::TOKEN_name      ("name");
const QString TaskListXmlHandler::TOKEN_t_elapsed ("time_elapsed");

TaskListXmlHandler::TaskListXmlHandler(QMap<int, Task*>* tl) :
    m_taskList(tl), m_temp_task(0), m_temp_task_name(QString("")),
    m_temp_task_elapsed(0), m_task_counter(-1)
{
}

bool TaskListXmlHandler::startDocument()
{
    qDebug() << "Znacznik - startDocument";
    actual_token = DOC_START;
    return true;
}

bool TaskListXmlHandler::startElement( const QString& ,
                                       const QString& ,
                                       const QString& qName,
                                       const QXmlAttributes& )
{
    qDebug() << "startElement " << qName;
    if (qName == TOKEN_task_list) {
        actual_token = TASK_LIST;

    } else if (qName == TOKEN_task) {
        actual_token = TASK;

    } else if (qName == TOKEN_name) {
        actual_token = NAME;

    } else if (qName == TOKEN_t_elapsed) {
        actual_token = T_ELAPSED;
    } else {
        actual_token = UNKNOWN;
        return true;
    }
    return true;
}

bool TaskListXmlHandler::characters(const QString& text)
{
    qDebug() << "characters " << text;
    switch (actual_token) {
        case DOC_START:
            break;
        case TASK_LIST:
            break;
        case TASK:
            break;
        case NAME:
            m_temp_task_name = text;
        case T_ELAPSED:
            m_temp_task_elapsed = text.toInt();
        default:            /* BLAD, nie znany token */
            return true;
    }
    return true;
}

bool TaskListXmlHandler::endElement( const QString& ,
                                     const QString& ,
                                     const QString& qName)
{
    qDebug() << "endElement " << qName;

    bool error = false;
    if (qName == TOKEN_task) {
        /* Nie zakonczono poprzedniego znacznika task. Prawdopodobnie 
         * niepoprawnie zapisany plik zadan */
        if (m_temp_task != 0) {
            qDebug() << "Niepoprawny wskaznik m_temp_task != 0";
            error = true;
        }

        /* Brak nazwy zadania */
        if (m_temp_task_name == QString("")) {
            qDebug() << "Brak nazwy zadania w m_temp_task_name";
            error = true;
        }

        /* UWAGA!, mozliwe jest zadanie ktore trwalo mniej niz 1 sek */
        if (m_temp_task_elapsed == 0) {
            qDebug() << "Zerowy czas zadania: m_temp_task_elapsed = 0";
            error = true;
        }

        if (error) {
            qDebug() << "BLAD parsowania pliku!";
            /*Jeden niepoprawny wpis nie musi przeszkodzic w dalszym czytaniu*/
            return true;
        }

        /* WSZYSTKO OK, dzialamy! */
        m_temp_task = new Task(m_temp_task_name, m_temp_task_elapsed);
        m_taskList->insert(m_task_counter, m_temp_task);
        m_temp_task = 0;
        --m_task_counter;
    }
    return true;
}

bool TaskListXmlHandler::fatalError(const QXmlParseException&)
{
    /* TODO */
    return true;
}

