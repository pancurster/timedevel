#ifndef TASK_LIST_WRITER_H
#define TASK_LIST_WRITER_H

//#include "task.h"

class Task;
//class QMap<int, Task*>;
class QString;
class QXmlStreamWriter;

class TaskListWriter {
    public:
        TaskListWriter(QMap<int, Task*>*, QString);

        /**
         * Zapisuje do pliku mape zadan w formacie XML
         */
        int write();

    private:
        QMap<int, Task*>* m_tToWrite;
        QString m_filename;
        QXmlStreamWriter* stream;
};

#endif // TASK_LIST_WRITER_H
