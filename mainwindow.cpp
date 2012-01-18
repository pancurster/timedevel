/* mainwindow.cpp */
#include <QDebug>
#include <QApplication>
#include <QWidget>
#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>
#include <QString>
#include <QMap>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>

#include <QxtGui/QxtWindowSystem>

#include "mainwindow.h"
#include "windowattr.h"
#include "task.h"
#include "tasklistwriter.h"
#include "tasklistreader.h"

MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent),
    m_taskInProgress(0)
{
    qDebug() << "Konstruktor MainWindow";

    m_topTasks = new QMap<int, Task*>;

    setUi();
}

void MainWindow::setUi()
{
    setWindowTitle("TimeDevelop 0.1v");

    QWidget* cw = new QWidget;
    setCentralWidget(cw);

    mainLayout = new QVBoxLayout;
    cw->setLayout(mainLayout);

    m_textedit = new QTextEdit("Empty");
    mainLayout->addWidget(m_textedit);
    m_textedit->setFixedSize(600, 300);

    buttonsLayout = new QHBoxLayout();
    mainLayout->addLayout(buttonsLayout);

    m_aktualizuj =  new QPushButton("Aktualizuj");
    m_zapisz =      new QPushButton("Zapisz");
    m_zakoncz =     new QPushButton("Zakoncz");
    m_wczytaj =     new QPushButton("Wczytaj liste");
    buttonsLayout->addWidget(m_aktualizuj);
    buttonsLayout->addWidget(m_zapisz);
    buttonsLayout->addWidget(m_wczytaj);
    buttonsLayout->addWidget(m_zakoncz);

    bool ret = connect(m_zakoncz, SIGNAL(clicked()), qApp, SLOT(quit()));
    Q_ASSERT(ret);
    ret = connect(m_aktualizuj, SIGNAL(clicked()), this, SLOT(
                                                    updateWidgetTaskList()));
    Q_ASSERT(ret);
    ret = connect(m_wczytaj, SIGNAL(clicked()), this, SLOT(readTaskListFile()));
    Q_ASSERT(ret);
    ret = connect(m_zapisz, SIGNAL(clicked()), this, SLOT(saveToFile()));
    Q_ASSERT(ret);
}

MainWindow::~MainWindow()
{
    delete m_textedit;
}

void MainWindow::processFocusChange()
{
    // Narazie kiepsko dziala
    //int WId = getActiveWId();

    WindowAttr* wa = new WindowAttr;
    int key_pid = wa->getPid();
    QString windowName = wa->getWName();

    Task* t = 0;
    if (m_topTasks->contains(key_pid)) {    /* Jest na liscie */
        if (m_taskInProgress->getWAttr()->getPid() != key_pid) {/*Zmiana zad.*/
            m_taskInProgress->stopTimer();
            m_taskInProgress = m_topTasks->value(key_pid);
            m_taskInProgress->startTimer();
        }
        /* Usuniecie wa powniewaz nie bedzie agregowany w m_topTasks */
        delete wa;
    } else {    /* Nowe zadanie */
        if (m_taskInProgress)
            m_taskInProgress->stopTimer();

        t = new Task(wa);
        m_topTasks->insert(wa->getPid(), t);
        m_taskInProgress = t;
        m_taskInProgress->startTimer();
    }
    updateWidgetTaskList();

    qDebug() << "Focus na oknie: " << windowName;
}

void MainWindow::updateWidgetTaskList()
{
    qDebug() << "Wywolanie MainWindow::updateWidgetTaskList";

    m_textedit->clear();
    QString task;
    const WindowAttr* wa;
    QMap<int, Task*>::iterator mi = m_topTasks->begin();

    while (mi != m_topTasks->end()) {
        wa = mi.value()->getWAttr();

        if (wa != 0) {
            task = QString("Okno: %1 ma WId: %2, PID: %3, aktywne sek.: %4")
                    .arg(wa->getWName())
                    .arg(QString::number(wa->getWId()))
                    .arg(QString::number(wa->getPid()))
                    .arg(QString::number(mi.value()->getElapsedTime()/1000));
        } else {
            task = QString("Okno: %1, aktywne sek.: %2")
                    .arg(mi.value()->getTaskName())
                    .arg(mi.value()->getElapsedTime()/1000);
        }
        m_textedit->append(task);
        ++mi;
    }
}

int MainWindow::getActiveWId()
{
    return QxtWindowSystem::activeWindow();
}

void MainWindow::saveToFile()
{
    TaskListWriter tlw(m_topTasks, "tasklist.xml");
    if (tlw.write())
        qDebug() << "Blad zapisu pliku";
}

void MainWindow::readTaskListFile()
{
    TaskListReader tlr(m_topTasks, "tasklist.xml");
    if (tlr.read())
        qDebug() << "Blad odczytu pliku";
}

