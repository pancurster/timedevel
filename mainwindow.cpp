/* mainwindow.cpp */
#include <QDebug>
#include <QApplication>
#include <QString>
#include <QStringList>
#include <QMap>

#include <QWidget>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include <QxtGui/QxtWindowSystem>

#include "mainwindow.h"
#include "windowattr.h"
#include "task.h"
#include "tasklistreader.h"
#include "taskmanager.h"

MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent),
    countTaskView(0), lastActiveTaskIndex(0)
{
    qDebug() << "Konstruktor MainWindow";

    setUi();
}

MainWindow::~MainWindow()
{
}

void MainWindow::readTaskListFile()
{
    /*
    TaskListReader tlr(m_topTasks, "tasklist.xml");
    if (tlr.read())
        qDebug() << "Blad odczytu pliku";
    */
}

void MainWindow::addTask(Task* t)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(m_taskView);
    item->setText(1, t->getTaskName());
    item->setText(4, QString::number(t->getElapsedTime()/1000));
    if (t->hasWAttr()) {
        const WindowAttr* wa = t->getWAttr();
        item->setText(2, QString::number(wa->getPid()));
        item->setText(3, QString::number(wa->getWId()));
    }
    m_taskView->insertTopLevelItem(countTaskView, item);
    ++countTaskView;

    qDebug() << "MainWindow::addTask: dodano zadanie do widoku";
}

void MainWindow::newActiveTask(Task* t)
{
    QList<QTreeWidgetItem* > match = m_taskView->findItems(t->getTaskName(),
                                                         Qt::MatchExactly, 1);
    if (match.isEmpty())
        return;
    m_taskView->topLevelItem(lastActiveTaskIndex)->setText(0,"");
    match[0]->setText(0, ">");
    lastActiveTaskIndex = m_taskView->indexOfTopLevelItem(match[0]);
}

void MainWindow::refreshElapsedTime(const QString& task, int newElapsedTime)
{
    qDebug() << "Wywolanie refreshElapsedTime" << task <<" "<<newElapsedTime;
    QList<QTreeWidgetItem* > match = m_taskView->findItems(task,
                                                          Qt::MatchExactly, 1);
    if (match.isEmpty()) {
        qDebug() << "BLAD!: refreshElapsedTime, nie znaleziono zadania"
                 << task;
        return;
    }
    match[0]->setText(4, QString::number(newElapsedTime/1000));
}

void MainWindow::setUi()
{
    setWindowTitle("TimeDevelop 0.1v");

    QWidget* cw = new QWidget;
    setCentralWidget(cw);

    mainLayout = new QVBoxLayout;
    cw->setLayout(mainLayout);

    m_taskView= new QTreeWidget;
    QStringList labels;
    labels << "A" <<"Task name" << "Pid" << "X Window Id" << "Time elapsed";
    m_taskView->setHeaderLabels(labels);
    mainLayout->addWidget(m_taskView);
    m_taskView->setFixedSize(600, 300);

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
    ret = connect(m_wczytaj, SIGNAL(clicked()),
            TaskManager::getInstance(), SLOT(readFromFile()));
    Q_ASSERT(ret);
    ret = connect(m_zapisz, SIGNAL(clicked()),
            TaskManager::getInstance(), SLOT(writeToFile()));
    Q_ASSERT(ret);
    /*
    ret = connect(m_aktualizuj, SIGNAL(clicked()),
                    this, SLOT(updateWidgetTaskList()));
    Q_ASSERT(ret);
    */
}

