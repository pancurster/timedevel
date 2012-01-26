#ifndef TASK_LIST_WRITER_H
#define TASK_LIST_WRITER_H

class Task;
class QString;
class QXmlStreamWriter;

/** Zapisuje stan modelu/zgromadzonych zdań z klasy typu \a TaskManager.
 * Plik zapisywany jest w formacie XML. Struktura jest następująca:
 * - nagłówek dokumentu XML
 * - znacznik dokumentu listy zadan programu timedevel: <timedevel_task_list>
 * - znacznik zadan głównych: <top_task>
 * - nastepnie dla kazdego zadania: znacznik zdania; <task>
 * - znaczniki danych: nazwy zadania <name>, nazwy aplikacji <app_name>,
 * nazwy zadania nadrzednego <parent>, znacznik czasu zadania <time_elapsed>
 * Kazde zadania zakończone odpowiednim znacznikiem konczącym np. </task>
 * - Nasepnie zapisywanie są zadania podrzędne: pomiędzy znacznikami 
 * otwierajacym <child_task> i odpowiednim zamukającym.
 * - znacznik zakonczenia dokumentu: </timedevel_task_list>
 */
class TaskListWriter {
    public:
        /** Konstruktor klasy TaskListWriter
         * @pram taskToWrite wskaźnik do mapy przechowywukacej zadania.
         * @pram filename nazwa pliku do zapisania. */
        TaskListWriter(QMap<QString, Task*>* taskToWrite,const QString filename);

        /** Zapisuje do pliku mape zadan w formacie XML */
        int write();

    private:
        QMap<QString, Task*>* m_tToWrite;
        QString m_filename;
        QXmlStreamWriter* stream;
};

#endif // TASK_LIST_WRITER_H
