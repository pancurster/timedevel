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

    signals:
        void taskAdded(Task*);
        void taskRemoved(const QString);
        void taskChanged(Task*);

    private:
        TaskManager(QObject* parent = 0);

        static TaskManager* theTaskManager;
};

#endif // TASK_MANAGER_H

