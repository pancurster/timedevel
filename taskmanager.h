#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <QObject>
#include <QMap>
#include <QString>
#include "task.h"

class TaskManager : public QObject, QMap<QString, Task*> {
    Q_OBJECT

    public:
        ~TaskManager();

        static TaskManager* getInstance();

    public slots:
        void add(const QString&, Task*);
        void remove(const QString& taskName);
        void setName(const QString& taskName, const QString& newName);
        void setElapsed(const QString& taskName, const int elapsed);
        void setActiveTask(const QString& taskName);

    signals:
        void taskAdded(Task*);
        void taskRemoved(const QString);
        void taskChanged(Task*);
        void newActiveTask(Task *);

    private:
        /** Obiekt konstruowany w getInstance() */
        TaskManager(QObject* parent = 0);

        /** Singleton */
        static TaskManager* theTaskManager;

        Task* m_activeTask;
};

#endif // TASK_MANAGER_H

