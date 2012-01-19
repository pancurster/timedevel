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
#include "taskview.h"
#include "taskmanager.h"

MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent),
    m_taskManager(0)
{
    qDebug() << "Konstruktor MainWindow";

    setUi();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setTaskModelManager(TaskManager* manager)
{
    if (m_taskManager == manager)
        return;
    if (m_taskManager) {
        disconnect(m_taskManager, SIGNAL(taskAdded(Task*)),
                   m_tViewTree, SLOT(addTaskView(Task*)));
        disconnect(m_taskManager, SIGNAL(newActiveTask(Task*)),
                   m_tViewTree, SLOT(newActiveTask(Task*)));
        disconnect(m_taskManager, SIGNAL(taskElapsedTimeChanged(const QString&, int)),
                   m_tViewTree, SLOT(refreshElapsedTime(const QString&, int)));
    }

    m_taskManager = manager;
    if (m_taskManager) {
        connect(m_taskManager, SIGNAL(taskAdded(Task*)),
                m_tViewTree, SLOT(addTaskView(Task*)));
        connect(m_taskManager, SIGNAL(newActiveTask(Task*)),
                m_tViewTree, SLOT(newActiveTask(Task*)));
        connect(m_taskManager, SIGNAL(taskElapsedTimeChanged(const QString&, int)),
                m_tViewTree, SLOT(refreshElapsedTime(const QString&, int)));
    }
}

void MainWindow::processFocusChange()
{
    // Narazie kiepsko dziala
    //int WId = getActiveWId();

    WindowAttr* wa = new WindowAttr;
    QString windowName = wa->getWName();
    Task* t = 0;

    if (!m_taskManager->isEmpty()
        && m_taskManager->getActive()->getTaskName() == windowName) {
        return;
    } else {
        if (m_taskManager->contains(windowName)) {
            m_taskManager->setActiveTask(windowName);
        } else {
            t = new Task(wa);
            m_taskManager->add(windowName, t);
            m_taskManager->setActiveTask(windowName);
            return;
        }
    }
    delete wa;
    qDebug() << "Focus na oknie: " << windowName;
}

int MainWindow::getActiveWId()
{
    return QxtWindowSystem::activeWindow();
}

void MainWindow::saveToFile()
{
    /*
    TaskListWriter tlw(m_topTasks, "tasklist.xml");
    if (tlw.write())
        qDebug() << "Blad zapisu pliku";
    */
}

void MainWindow::readTaskListFile()
{
    /*
    TaskListReader tlr(m_topTasks, "tasklist.xml");
    if (tlr.read())
        qDebug() << "Blad odczytu pliku";
    */
}

void MainWindow::setUi()
{
    setWindowTitle("TimeDevelop 0.1v");

    QWidget* cw = new QWidget;
    setCentralWidget(cw);

    mainLayout = new QVBoxLayout;
    cw->setLayout(mainLayout);

    m_tViewTree = new TaskView;
    mainLayout->addWidget(m_tViewTree);
    m_tViewTree->setFixedSize(600, 300);

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

    bool ret;
    ret = connect(m_zakoncz, SIGNAL(clicked()), qApp, SLOT(quit()));
    Q_ASSERT(ret);
    ret = connect(m_wczytaj, SIGNAL(clicked()),this, SLOT(readTaskListFile()));
    Q_ASSERT(ret);
    ret = connect(m_zapisz, SIGNAL(clicked()), this, SLOT(saveToFile()));
    Q_ASSERT(ret);
    /*
    ret = connect(m_aktualizuj, SIGNAL(clicked()),
                    this, SLOT(updateWidgetTaskList()));
    Q_ASSERT(ret);
    */
}

