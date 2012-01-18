#ifndef TASK_H
#define TASK_H

class QTime;
class QString;
class Task;
class WindowAttr;

class Task {
    public:
        Task(WindowAttr* wa);
        Task(const QString taskName);
        Task(const QString taskName, const int t_elapsed);
        ~Task();

        /** Zwraca nazwe zadania */
        QString getTaskName();
        /** Zwraca stala do atrybutow okna */
        const WindowAttr* getWAttr();
        /** Zwraca true jesli zadanie podiada poprawny wskaznik m_wattr*/
        bool hasWAttr();
        /** Zwraca sumaryczny czas spedzony na zadaniu */
        int getElapsedTime();
        /** Zwraca sumaryczny czas spedzony na zadaniu jako QString */
        QString getElapsedTimeString();
        /** Zaczyna zliczanie czasu dla zadania */
        void startTimer();
        /** Zatrzymuje zliczanie czasu */
        void stopTimer();
        /** Ustawie nazwe zadania */
        void setName(const QString& taskName);
        /** Ustawia czas spedzony na zadaniu */
        void setElapsed(int elapsed);

    private:
        QString m_taskName;
        QTime* m_time;
        int m_elapsed;
        //QString m_elapsed;

        WindowAttr* m_wattr;
        Task* m_parentTask;
};

#endif // TASK_H
