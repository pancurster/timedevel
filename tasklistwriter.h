#ifndef TASK_LIST_WRITER_H
#define TASK_LIST_WRITER_H

//#include "task.h"

class Task;
//class QMap<int, Task*>;
class QString;
class QXmlStreamWriter;

class TaskListWriter {
    public:
        TaskListWriter(QMap<QString, Task*>*, const QString);

        /**
         * Zapisuje do pliku mape zadan w formacie XML
         */
        int write();

    private:
        QMap<QString, Task*>* m_tToWrite;
        QString m_filename;
        QXmlStreamWriter* stream;
};

#endif // TASK_LIST_WRITER_H
