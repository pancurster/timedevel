#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

class Task;
class TaskTreeWidget;

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

        TaskTreeWidget* getMainWidget();

    private slots:
        void trayIconClicked(QSystemTrayIcon::ActivationReason reason);

    signals:
        void orderQuit();
        void orderPreferences();

        void offFocusDetector();
        void onFocusDetector();

    private:
        void setUi();
        void setToolbar();
        void setConnections();

        TaskTreeWidget*  m_taskTreeWidget;  //Main widget

        QSystemTrayIcon* m_trayIcon;        //Ikona w trayu

        QAction*         m_newTaskAction;
        QAction*         m_deleteTaskAction;
        QAction*         m_findTaskAction;
        QAction*         m_editTaskAction;
        QAction*         m_perferencesAction;
        QAction*         m_quitAction;

//        enum {ACTIVE_C, TASK_N_C, APP_N_C, ELAPS_C,
//              PID_C, WID_C} columnOrder;
};

#endif // MAINWINDOW_H

