#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <QObject>
#include <QMap>
#include <QString>
#include "task.h"

class TaskManager : public QObject, public QMap<QString, Task*> {
    Q_OBJECT

    public:
        ~TaskManager();

        static TaskManager* getInstance();
        Task* getActive();
        void setActiveTask(const QString& taskName);

    public slots:
        void add(const QString&, Task*);
        void add(const QString&);
        void remove(const QString& taskName);
        void setName(const QString& taskName, const QString& newName);
        void setElapsed(const QString& taskName, const int elapsed);
        void writeToFile();
        void readFromFile();

    signals:
        void taskAdded(Task*);
        void taskRemoved(const QString&);
        void taskElapsedTimeChanged(const QString&, int);
        void taskChanged(Task*);
        void newActiveTask(Task *);
        void writeDone(bool);
        void readDone(bool);

    private:
        /** Obiekt konstruowany w getInstance() */
        TaskManager(QObject* parent = 0);

        /** Probuje znalezc (w istniejacych zadaniach) i ustawic rodzica
         * zadania. Funkcja powinna byc wywolywane kiedy zadanie nie ma 
         * jeszcze rodzica (najczesciej wystepuje przy wykryciu nowego programu).
         */
        void trySetParent(Task* t);

        /** Singleton */
        static TaskManager* theTaskManager;

        Task* m_activeTask;
};

#endif // TASK_MANAGER_H

