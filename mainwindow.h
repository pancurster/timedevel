#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QTextEdit;
class Task;

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
        /** Odczytuje aktualnie aktywne okno i akutalizuje 
         *  dane o aktywynym oknie w mainwidget */
        void processFocusChange();

        /** Aktualizuje widget wyswietlajacy zadania */
        void updateWidgetTaskList();

        /** Zapisuje do pliku liste zadan */
        void saveToFile();

        /** Odczytuke liste zadan z pliku */
        void readTaskListFile();

    private:
        int getActiveWId();
        void setUi();

        QMap<int, Task*>* m_topTasks;
        Task* m_taskInProgress;

        /**** GUI ***/

        QLabel* m_label;
        QTextEdit* m_textedit;
        QPushButton* m_aktualizuj, *m_zapisz, *m_zakoncz, *m_wczytaj;

        QVBoxLayout* mainLayout;
        QHBoxLayout* buttonsLayout;
        QGridLayout* glayout;

};

#endif // MAINWINDOW_H
