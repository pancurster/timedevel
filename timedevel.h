#ifndef TIMEDEVEL_H
#define TIMEDEVEL_H

#include <QObject>

class TaskManager;
class TaskView;

class Timedevel : public QObject {

    Q_OBJECT

    public:
        Timedevel(TaskManager* tm, TaskView* view, QObject* parent = 0);
        ~Timedevel();

        /** Ustawia model danych */
        void setTaskModel(TaskManager* manager);

        /** Ustawia widok */
        //TODO Kontener? Kilka widokow? (Ma byc wyswietlanie 
        //statystyk jako grafiki.
        void setTaskView(TaskView* taskviewer);

    public slots:
        /** Odczytuje aktualnie aktywne okno i akutalizuje 
         *  dane o aktywynym oknie w mainwidget */
        void processFocusChange();

    private:
        /** Ma pobierac info o oknie. Narazie zle dziala :) */
        int getActiveWId();

        /** MVC - model */
        TaskManager* m_taskManager;
        /** MVC - widok */
        TaskView* m_tViewTree;
};

#endif // TIMEDEVEL_H
