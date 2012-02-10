#ifndef TIMEDEVEL_H
#define TIMEDEVEL_H

#include <memory>

#include <QObject>
#include <QString>

#include "options_parsing.h"

class TaskManager;
class MainWindow;
class FocusDetector;
class FocusActivator;
class TimerActivator;

class Timedevel : public QObject {

    Q_OBJECT

    public:
        Timedevel(TaskManager* tm,
                  MainWindow* view,
                  FocusDetector* fdo,
                  std::auto_ptr<Options> opt,
                  QObject* parent = 0);
        ~Timedevel();

        /** Ustawia model danych */
        void setTaskModel(TaskManager* manager);

        /** Ustawia widok */
        void setTaskView(MainWindow* taskviewer);

        void setTaskFile(const std::string& taskfile);
        void readTaskFile();
        void writeTaskFile();

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

        /// Ustawieia programu z CLI
        void parseOptions(OptionsMap* options);

        /** MVC - model */
        TaskManager* m_taskManager;
        /** MVC - widok */
        MainWindow* m_mainView;

        FocusDetector* m_focusDetector;
        TimerActivator* m_timerActivator;

        QString m_taskFile;
};

#endif // TIMEDEVEL_H
