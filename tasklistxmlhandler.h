#ifndef TASK_LIST_XML_HANDLER
#define TASK_LIST_XML_HANDLER

#include <QString>
#include <QXmlDefaultHandler>
class Task;
class TaskManager;

/** Handler dla klasy QXmlSimpleReader.
 * Logika odczytywania pliku i przywracania zadań do kontenera TaskManager
 * Wszystkie publiczne funkcje tej klasy zasłaniają interfejs klasy 
 * QXmlDefaultHandler po której dzidziczy \a TaskListXmlHandler. Żadna z 
 * funkcji składowych pub. nie powinna być wywoływana ręcznie.
 * Struktura pliku opisana jest w \a TaskListWriter.
 * @see QXmlDefaultHandler
 */
class TaskListXmlHandler : public QXmlDefaultHandler {
/* TODO: Dodac poprawna obsluge bledow - klasy QXmlErrorHandler */
    public:
        TaskListXmlHandler(TaskManager*);
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
        /* Sprawdza czy odczytane dane dla zadania są porawne */
        bool isDataCorrect();

        /* Wskaznik do agregatora zadan. Podawany przez klienta klasy */
        TaskManager* m_taskList;

        /* Tymczasowy wskaznik na Task(zadanie). Podczas czytania pliku jest mu
         * przypisywana wartosc zmiennej Task alokowanej na stercie, nastepnie
         * zwalnianie pamieci spada na kontener m_taskList */
        Task* m_temp_task;

        /* Tymczasowa wartosc przechowywujaca nazwe zadania */
        QString m_temp_task_name;

        /** Tymczasowa wartosc przechowywujaca nazwe aplikacji powiazanej
         * z danym zadaniem */
        QString m_temp_task_app_name;

        /* Tymczasowa wartosc przechowywujaca czas spedzony na zadaniu */
        int m_temp_task_elapsed;

        /** Rodzic zadania */
        QString m_temp_task_parent;

        /* Wszystkie mozliwe tokeny w xmlu. Definicja w pliku cpp */
        static const QString TOKEN_top_task;
        static const QString TOKEN_child_task;
        static const QString TOKEN_task;
        static const QString TOKEN_name;
        static const QString TOKEN_app_name;
        static const QString TOKEN_parent;
        static const QString TOKEN_t_elapsed;

        /* Sluzy do rozpoznawania aktualnie parsowanego tokenu */
        enum token_t { UNKNOWN = -1,
                       DOC_START,
                       TOP_TASK, 
                       CHILD_TASK,
                       TASK, 
                       NAME, 
                       APP_NAME,
                       PARENT,
                       T_ELAPSED } actual_token;
        
};

#endif // TASK_LIST_XML_HANDLER
