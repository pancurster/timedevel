#ifndef TASK_H
#define TASK_H

class QTime;
class QString;
class Task;
class WindowAttr;

/**
 * \class Task
 * Klasa \a Task - podstawowa klasa przechowywujaca parametry
 * uruchomionych programow i zadan. Agregowana przez klase
 * \a TaskManager
 */
class Task {
    public:
        Task(WindowAttr* wa);
        Task(const QString taskName);
        /** Tworzy nowa instancje klasy \a Task
         * \param taskName nazwa zadania lub programu
         * \param t_elapsed czas spedzony na zadaniu
         */
        Task(const QString taskName, const int t_elapsed);
        ~Task();

        // GETERS
        /** Zwraca nazwe aplikacji z ktora powiazane jest zadanie */
        QString getAppName();
        /** Zwraca sumaryczny czas spedzony na zadaniu */
        int getElapsedTime();
        /** Zwraca sumaryczny czas spedzony na zadaniu jako QString */
        QString getElapsedTimeString();
        /** Zwraca paretn task zadania */
        Task* getParent();
        /** Zwraca nazwe zadania */
        QString getTaskName();
        /** Zwraca stala do atrybutow okna */
        const WindowAttr* getWAttr();

        // INFO
        /** Czy zadania ma rodzica? */
        bool hasParent();
        /** Sprawdza czy obiekt posiada poprawne dane w \a WindowAttr */
        bool hasWAttr();

        // SETERS
        /** Ustawia czas spedzony na zadaniu */
        void setElapsed(int elapsed);
        /** Ustawie nazwe zadania */
        void setName(const QString& taskName);
        /** Ustawia parent task zadania */
        void setParent(Task* t);
        /** Zaczyna zliczanie czasu dla zadania */
        void startTimer();
        /** Zatrzymuje zliczanie czasu */
        void stopTimer();

    private:
        QString m_taskName;
        QString m_appName;
        int m_elapsed;
        QTime* m_time;

        WindowAttr* m_wattr;
        Task* m_parentTask;
};

#endif // TASK_H
