#ifndef TASK_H
#define TASK_H

class Task;
class QTime;
class QString;
class QList<Task>;
class WindowAttr;

/**
 * \class Task
 * Klasa \a Task - podstawowa klasa przechowywujaca parametry
 * uruchomionych programow i zadan. Agregowana przez klase
 * \a TaskManager
 */
class Task {
    public:
        /** Tworzy zadanie tylko na podstawie atrybutów okna */
        Task(WindowAttr* wa);

        /** Kontruktor urzywany do tworzenia pustego zadania.
         * Zadanie stworzone przez ten konstruktor nie ma powiązania z żadną
         * aplikacją. 
         * @param taskName nazwa zadania.
         */
        Task(const QString taskName);

        /** Tworzy nowa instancje klasy \a Task
         * @param taskName nazwa zadania lub programu
         * @param t_elapsed czas spedzony na zadaniu
         */
        Task(const QString taskName, const int t_elapsed);
        ~Task();

        // GETERS
        /** Zwraca nazwe aplikacji z ktora powiazane jest zadanie */
        QString getAppName();
        /** Zwraca nazwe zadania */
        QString getTaskName();
        /** Zwraca sumaryczny czas spedzony na zadaniu jako QString */
        QString getElapsedTimeString();
        /** Zwraca paretn task zadania */
        Task* getParent();
        /** Zwraca stala do atrybutow okna */
        const WindowAttr* getWAttr();
        /** Zwraca sumaryczny czas spedzony na zadaniu */
        int getElapsedTime();
        /// Zwraca sumaryczny czas pod-zadan
        int getElpasedChildrenTime();
        /// Zwraca sume czasu zadania i dzieci
        int getAllElapsedTime();

        // INFO
        /** Czy zadania ma rodzica? 
         * @return true jestli zadanie ma poprawnie ustawionego rodzica
         * @see m_parentTask. */
        bool hasParent();
        /** Sprawdza czy obiekt posiada poprawne dane w \a WindowAttr 
         * @return true jeśli zadanie ma poprawnie ustawiony wskaźnik na
         * atrybuty okna. */
        bool hasWAttr();

        // SETERS
        /** Ustawia nzwe aplikacji powiazanej z zadaniem 
         * @param appName nazwa aplikacji powiazanej z zadaniem. */
        void setAppName(const QString& appName);
        /** Ustawia czas spedzony na zadaniu 
         * @param elapsed łączny czas spedzony na zadaniu. */
        void setElapsed(int elapsed);
        /** Ustawie nazwe zadania 
         * @param taskName nowa nazwa zadania */
        void setName(const QString& taskName);
        /** Ustawia rodzica zadania na t 
         * @param t wskaźnik do zadania nadrzędnego/rodzica. */
        void setParent(Task* t);

        /// Dodaje pod-zadanie
        void addChildTask(Task* t);
        /// Usuwa pod-zadanie
        void removeChildTask(Task* t);
        /// Zwraca liste dzieci
        QList<Task*> getChildren();
        /// Zaczyna zliczanie czasu dla zadania
        void startTimer();
        /// Zatrzymuje zliczanie czasu
        void stopTimer();

    private:
        /// Dodaje do czas dziecka do licznika czasu dzieci
        void addChildTime(int deltaTime);
        /// Usuwa czas dziecka z licznika czasu dzieci
        void removeChildTime(int childElapsedTime);

        QString m_taskName; ///< Nazwa zadania/tytul okna.
        QString m_appName;  ///< Nazwa aplikacji powiazanej z zadaniem.

        int m_elapsed;      ///< Łączny czas spędzony na zadaniu.
        int m_childrenElapsed; ///< Czas dzieci zadania
        QTime* m_time;      ///< Liczy czas spedzony na zadaniu.

        WindowAttr* m_wattr;///< Jesli zadanie jest aplikacja, posiada atrybuty
                            ///< okna (PID, WId itp).
        Task* m_parentTask; ///< Wskaźnik do zadania nadrzędnego/rodzica.
        QList<Task*>* m_childrenTask;
};

#endif // TASK_H
