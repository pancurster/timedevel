#ifndef TIMEDEVEL_H
#define TIMEDEVEL_H

#include <QObject>

class TaskManager;
class MainWindow;
class FocusDetector;
class FocusActivator;

class Timedevel : public QObject {

    Q_OBJECT

    public:
        Timedevel(TaskManager* tm, MainWindow* view,
                  FocusDetector* fdo, QObject* parent = 0);
        ~Timedevel();

        /** Ustawia model danych */
        void setTaskModel(TaskManager* manager);

        /** Ustawia widok */
        //TODO Kontener? Kilka widokow? (Ma byc wyswietlanie 
        //statystyk jako grafiki.
        void setTaskView(MainWindow* taskviewer);

        void addFocusActivator(FocusActivator* f_activator);
        void removeFocusActivator(FocusActivator* f_activator);

    public slots:
        /** Odczytuje aktualnie aktywne okno i akutalizuje 
         *  dane o aktywynym oknie w mainwidget */
        void processFocusChange();

        /// Wlaczanie, wylaczanie detekcji zmiany focusa
        void offFocusDetector();
        void onFocusDetector();
    private:
        /** Ma pobierac info o oknie. Narazie zle dziala :) */
        int getActiveWId();

        /** MVC - model */
        TaskManager* m_taskManager;
        /** MVC - widok */
        MainWindow* m_taskView;

        FocusDetector* m_focusDetector;
};

#endif // TIMEDEVEL_H
