#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QTextEdit;
class Task;
class TaskView;
class TaskManager;

class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;

class MainWindow : public QMainWindow {

    Q_OBJECT

    public:
        MainWindow(QWidget* parent = 0);
        ~MainWindow();

        TaskView* getView();

    public slots:
        /** Zapisuje do pliku liste zadan */
        void saveToFile();

        /** Odczytuke liste zadan z pliku */
        void readTaskListFile();

    private:
        void setUi();

        //TaskManager* m_taskManager;

        /**** GUI ***/

        QLabel* m_label;
        TaskView* m_tViewTree;
        QPushButton* m_aktualizuj, *m_zapisz, *m_zakoncz, *m_wczytaj;

        QVBoxLayout* mainLayout;
        QHBoxLayout* buttonsLayout;
        QGridLayout* glayout;

};

#endif // MAINWINDOW_H
