//tasklistxmlhandler.cpp
#include <QDebug>
#include <QMap>
#include <QXmlAttributes>
#include <QXmlParseException>

#include "task.h"
#include "tasklistxmlhandler.h"
#include "taskmanager.h"

const QString TaskListXmlHandler::TOKEN_top_task  ("top_task");
const QString TaskListXmlHandler::TOKEN_child_task("child_task");
const QString TaskListXmlHandler::TOKEN_task      ("task");
const QString TaskListXmlHandler::TOKEN_name      ("name");
const QString TaskListXmlHandler::TOKEN_app_name  ("app_name");
const QString TaskListXmlHandler::TOKEN_parent    ("parent");
const QString TaskListXmlHandler::TOKEN_t_elapsed ("time_elapsed");

TaskListXmlHandler::TaskListXmlHandler(TaskManager* tl) :
    m_taskList(tl), m_temp_task(0), m_temp_task_name(QString("")),
    m_temp_task_app_name(""), m_temp_task_elapsed(0),
    m_temp_task_parent("")
{
}

bool TaskListXmlHandler::startDocument()
{
    qDebug() << Q_FUNC_INFO;
    actual_token = DOC_START;
    return true;
}

bool TaskListXmlHandler::startElement( const QString& ,
                                       const QString& ,
                                       const QString& qName,
                                       const QXmlAttributes& )
{
    qDebug() << Q_FUNC_INFO << "Element " << qName;
    if        (qName == TOKEN_top_task) {
        actual_token = TOP_TASK;

    } else if (qName == TOKEN_child_task) {
        actual_token = CHILD_TASK;
        
    } else if (qName == TOKEN_task) {
        actual_token = TASK;

    } else if (qName == TOKEN_name) {
        actual_token = NAME;

    } else if (qName == TOKEN_app_name) {
        actual_token = APP_NAME;

    } else if (qName == TOKEN_parent) {
        actual_token = PARENT;

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
    qDebug() << Q_FUNC_INFO << "characters " << text;
    switch (actual_token) {
        case DOC_START:
            break;
        case TOP_TASK:
            break;
        case CHILD_TASK:
            break;
        case TASK:
            break;
        case NAME:
            m_temp_task_name = text;
        case APP_NAME:
            m_temp_task_app_name = text;
        case PARENT:
            m_temp_task_parent = text;
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
    qDebug() << Q_FUNC_INFO << "Element " << qName;

    if (qName == TOKEN_task) {
        if ( ! isDataCorrect()) {
            qDebug() << "   BLAD parsowania pliku!" << Q_FUNC_INFO;
            /*Jeden niepoprawny wpis nie musi przeszkodzic w dalszym czytaniu*/
//            return false;
        }

        /* WSZYSTKO OK, dzialamy! */
        m_temp_task = new Task(m_temp_task_name, m_temp_task_elapsed);
        m_temp_task->setAppName(m_temp_task_app_name);
        if (m_temp_task_parent != "None") {
            Task* parent_t = m_taskList->value(m_temp_task_parent);
            if ( parent_t != 0) {
                m_temp_task->setParent(parent_t);
            } else {
                qDebug() <<"   BLAD: Nie znaleziono rodzica "
                         <<m_temp_task_parent <<" zadania: "<<m_temp_task_name
                         <<Q_FUNC_INFO;
            }
        }

        m_taskList->add(m_temp_task_name, m_temp_task);
        m_temp_task = 0;
    }
    return true;
}

bool TaskListXmlHandler::fatalError(const QXmlParseException&)
{
    /* TODO */
    return true;
}

bool TaskListXmlHandler::isDataCorrect()
{
    bool dataCorrect = true;
    /* Nie zakonczono poprzedniego znacznika task. Prawdopodobnie 
     * niepoprawnie zapisany plik zadan */
    if (m_temp_task != 0) {
        qDebug() << "Niepoprawny wskaznik m_temp_task != 0";
        dataCorrect = false;
    }
    /* Brak nazwy zadania */
    if (m_temp_task_name == QString("")) {
        qDebug() << "Brak nazwy zadania w m_temp_task_name";
        dataCorrect = false;
    }
    /* UWAGA!, mozliwe jest zadanie ktore trwalo mniej niz 1 sek */
    if (m_temp_task_elapsed == 0) {
        qDebug() << "Zerowy czas zadania: m_temp_task_elapsed = 0";
        dataCorrect = false;
    }
    /* Brak rodzica lub niepoprawny wpis w <parent> */
    if (m_temp_task_parent == QString("")
        || m_temp_task_parent != QString("None")) {
        qDebug() << "Niepoprawna definicja rodzica zadania";
        dataCorrect = false;
    } 
    return dataCorrect;
}

