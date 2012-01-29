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
        void editTaskName(QTreeWidgetItem* item, int column);
        void endEditTaskName(QTreeWidgetItem* item, int column);
        void trayIconClicked(QSystemTrayIcon::ActivationReason reason);

    signals:
        void orderQuit();
        void orderLoad();
        void orderSave();
        void orderNewTask();
        void orderEditTaskName(const QString& taskName, const QString& newName);
        void orderDeleteTask(const QString* taskName);
        void orderFindTask(const QString* taskName);
        void orderPreferences();

    private:
        void setUi();
        void setToolbar();
        void setConnections();

        QTreeWidgetItem* findItem(const QString& tName);
        QTreeWidgetItem* findItem(Task* t);
        QString          toMinSec(int counter, const QString spliter = ":");

        QTreeWidget*     m_taskView;        //Main widget
        QSystemTrayIcon* m_trayIcon;        //Ikona w trayu
        QToolBar*        m_toolbar;         //Pasek narzedziowy
        QAction*         m_newTaskAction;   //Nowe zadanie
        QAction*         m_deleteTaskAction;
        QAction*         m_findTaskAction;
        QAction*         m_editTaskAction;
        QAction*         m_perferencesAction;
        QPushButton*     m_aktualizuj;
        QPushButton*     m_zapisz;
        QPushButton*     m_zakoncz;
        QPushButton*     m_wczytaj;

        QTreeWidgetItem* m_activeItem;      //Aktualnie aktywne zadanie
        QString          m_editedTaskName;  //Nazwa edytowanego zadania
        bool             m_editorActive;    //Flaga aktywnego edytora

        // Kolejnosc pol w wyliczeniu wyznacza kolejnosc w m_taskView typu
        // QTreeWidget. Zabezpiecza przed rzaglerka cygerkami w funkcjach
        // ktore oczekuja numeru kolumny widgetu QTreeWidget.
        enum {ACTIVE_C, TASK_N_C, APP_N_C, ELAPS_C,
              PID_C, WID_C} columnOrder;
};

#endif // MAINWINDOW_H

