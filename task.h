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

        /** Zwraca nazwe zadania */
        QString getTaskName();
        /** Zwraca stala do atrybutow okna */
        const WindowAttr* getWAttr();
        /** Sprawdza czy obiekt posiada poprawne dane w \a WindowAttr 
         * \return true jesli zadanie posiada poprawnie zainicjalizowany
         * wskaznik m_wattr \a WindowAttr
         */
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
        /** Ustawia parent task zadania */
        void setParent(Task* t);
        /** Zwraca paretn task zadania */
        Task* getParent();
        /** Czy zadania ma rodzica? */
        bool hasParent();

    private:
        QString m_taskName;
        QTime* m_time;
        int m_elapsed;
        //QString m_elapsed;

        WindowAttr* m_wattr;
        Task* m_parentTask;
};

#endif // TASK_H
