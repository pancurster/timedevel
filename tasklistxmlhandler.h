#ifndef TASK_LIST_XML_HANDLER
#define TASK_LIST_XML_HANDLER

#include <QString>
#include <QXmlDefaultHandler>
class Task;
//class QString;

/* TODO: Dodac poprawna obsluge bledow - klasy QXmlErrorHandler */
class TaskListXmlHandler : public QXmlDefaultHandler {
    public:
        TaskListXmlHandler(QMap<int, Task*>*);
        bool startDocument();
        bool startElement( const QString& namespaceURI,
                           const QString& localName,
                           const QString& qName,
                           const QXmlAttributes& atts);
        bool characters(const QString& text);
        bool endElement( const QString& namespaceURI,
                         const QString& localName,
                         const QString& qName);

        bool fatalError(const QXmlParseException& exception);
    private:
        /* Wskaznik do agregatora zadan. Podawany przez klienta klasy */
        QMap<int, Task*>* m_taskList;

        /* Tymczasowy wskaznik na Task(zadanie). Podczas czytania pliku jest mu
         * przypisywana wartosc zmiennej Task alokowanej na stercie, nastepnie
         * zwalnianie pamieci spada na kontener m_taskList */
        Task* m_temp_task;

        /* Tymczasowa wartosc przechowywujaca nazwe zadania */
        QString m_temp_task_name;

        /* Tymczasowa wartosc przechowywujaca czas spedzony na zadaniu */
        int m_temp_task_elapsed;

        /* Przechowuje liczbe odczytanych zadan. Poniewaz nie znane sa 
         * PIDy zadan oraz za kazdym uruchomieniem, PID zadania zmienia sie,
         * licznik ten odlicza do tylu dla rozroznienia i jest kluczem 
         * w m_taskList.
         */
        int m_task_counter;

        /* Wszystkie mozliwe tokeny w xmlu */
        static const QString TOKEN_task_list;
        static const QString TOKEN_task;
        static const QString TOKEN_name;
        static const QString TOKEN_t_elapsed;

        /* Sluzy do rozpoznawania aktualnie parsowanego tokenu */
        enum token_t { UNKNOWN = -1,
                       DOC_START,
                       TASK_LIST, 
                       TASK, 
                       NAME, 
                       T_ELAPSED } actual_token;
        
};

#endif // TASK_LIST_XML_HANDLER
