#ifndef TASK_LIST_READER
#define TASK_LIST_READER

class Task;
class QString;

class TaskListReader {
    public:
        TaskListReader(QMap<int, Task*>*, QString);

        int read();

    private:
        QMap<int, Task*>* m_readDest;
        QString m_filename;
};

#endif // TASK_LIST_READER
