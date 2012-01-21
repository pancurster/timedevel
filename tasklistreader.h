#ifndef TASK_LIST_READER
#define TASK_LIST_READER

class Task;
class QString;
class TaskManager;

class TaskListReader {
    public:
        TaskListReader(TaskManager*, QString);

        int read();

    private:
        TaskManager* m_readDest;
        QString m_filename;
};

#endif // TASK_LIST_READER
