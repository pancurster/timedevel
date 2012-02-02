#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

class Task;

class QTreeWidget;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSystemTrayIcon;
class QTreeWidgetItem;

class MainWindow : public QMainWindow {

    Q_OBJECT

    public:
        MainWindow(QWidget* parent = 0);
        ~MainWindow();

    public slots:
        void addTask(Task* t);
        void newActiveTask(Task* t);
        void refreshElapsedTime(const QString& task, int newElapsedTime);
        void removeTaskFromView(const QString& taskName);
        void trayIconClicked(QSystemTrayIcon::ActivationReason reason);
        
    private slots:
        void processRemoveTask();   // emits orderRemoveTask(...)
        void processNewTask();
        void processEditTaskName();
        void processFindTask();

    signals:
        void orderQuit();
        void orderLoad();
        void orderSave();
        void orderNewTask(const QString& taskName);
        void orderEditTaskName(const QString& taskName, const QString& newName);
        void orderRemoveTask(const QString& taskName);
        void orderFindTask(const QString& taskName);
        void orderPreferences();

        void offFocusDetector();
        void onFocusDetector();

    private:
        void setUi();
        void setToolbar();
        void setConnections();

        QTreeWidgetItem* findItem(const QString& tName);
        QTreeWidgetItem* findItem(Task* t);
        QTreeWidgetItem* findTopLevelItem(const QString& taskName);
        QString          toMinSec(int counter, const QString spliter = ":");

        QTreeWidget*     m_taskView;        //Main widget
        QSystemTrayIcon* m_trayIcon;        //Ikona w trayu

        QAction*         m_newTaskAction;
        QAction*         m_deleteTaskAction;
        QAction*         m_findTaskAction;
        QAction*         m_editTaskAction;
        QAction*         m_perferencesAction;
        QAction*         m_quitAction;

        QTreeWidgetItem* m_activeItem;      //Aktualnie aktywne zadanie

        // Kolejnosc pol w wyliczeniu wyznacza kolejnosc w m_taskView typu
        // QTreeWidget. Zabezpiecza przed rzaglerka cygerkami w funkcjach
        // ktore oczekuja numeru kolumny widgetu QTreeWidget.
        enum {ACTIVE_C, TASK_N_C, APP_N_C, ELAPS_C,
              PID_C, WID_C} columnOrder;
};

#endif // MAINWINDOW_H

