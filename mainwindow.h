#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Task;

class QTreeWidget;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;

class MainWindow : public QMainWindow {

    Q_OBJECT

    public:
        MainWindow(QWidget* parent = 0);
        ~MainWindow();

    public slots:
        /** Odczytuke liste zadan z pliku */
        void readTaskListFile();

        void addTask(Task* t);
        void newActiveTask(Task* t);
        void refreshElapsedTime(const QString& task, int newElapsedTime);

    private:
        void setUi();

        int countTaskView;
        int lastActiveTaskIndex;

        /**** GUI ***/

        /* Main widget */
        QTreeWidget* m_taskView;

        QPushButton* m_aktualizuj, *m_zapisz, *m_zakoncz, *m_wczytaj;

        /* Layouts */
        QVBoxLayout* mainLayout;
        QHBoxLayout* buttonsLayout;
        QGridLayout* glayout;

};

#endif // MAINWINDOW_H
