#ifndef TASK_LIST_READER
#define TASK_LIST_READER

class Task;
class QString;
class TaskManager;

/** Odczytuje z pliku, zapisane zadania z poprzedniej sesji.
 * Klasa ta agreguje obiekt QXmlSimpleReader, który do działania potrzebuje
 * tzw. handlera - definicję i logikę odczytywania pliku.
 * Używany handler \a TaskListXmlHandler dziedziczy po QXmlDefaultHandler z
 * biblioteki Qt.
 * Plik zapisywany jest w formacie XML.
 * @see TaskListXmlHandler
 * @see QXmlDefaultHandler
 * @see QXmlSimpleReader
 */
class TaskListReader {
    public:
        /** Konstrukor klasy.
         * @param tm wskaźnik do obiektu klasy TaskManager - kontenera zadań.
         * @param filename Nazwa pliku z którego należy czytać zadania. */
        TaskListReader(TaskManager* tm, QString filename);

        /** Rozpoczyna czytanie pliku i ładowanie zadań do \a tm */
        int read();

    private:
        TaskManager* m_readDest;    ///< Wskaźnik na kontener zdań/model.
        QString m_filename;         ///< Nazw pliku do odczytania.
};

#endif // TASK_LIST_READER
