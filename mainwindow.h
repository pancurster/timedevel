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
        void zakonczClicked();
        void wczytajClicked();
        void zapiszClicked();
        void newTaskName(const QString& taskName, const QString& newName);

    private:
        void setUi();
        QTreeWidgetItem* findItem(const QString& tName);
        QTreeWidgetItem* findItem(Task* t);
        QString toMinSec(int counter, const QString spliter = ":");

        QTreeWidgetItem* m_activeItem;  //aktualnie aktywne zadanie
        QString m_editedTaskName;       //nazwa edytowanego zadania
        bool m_editorActive;            //flaga aktywnego edytora

        // Kolejnosc pol w wyliczeniu wyznacza kolejnosc w m_taskView typu
        // QTreeWidget. Zabezpiecza przed rzaglerka cygerkami w funkcjach
        // ktore oczekuja numeru kolumny widgetu QTreeWidget.
        enum {ACTIVE_C, TASK_N_C, APP_N_C, ELAPS_C,
              PID_C, WID_C} columnOrder;

        /* Main widget */
        QTreeWidget* m_taskView;

        QPushButton* m_aktualizuj, *m_zapisz, *m_zakoncz, *m_wczytaj;
        QSystemTrayIcon* m_trayIcon;

        /* Layouts */
        QVBoxLayout* mainLayout;
        QHBoxLayout* buttonsLayout;
        QGridLayout* glayout;

};

#endif // MAINWINDOW_H

