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
    QMainWindow(parent)
{
    qDebug() << "Konstruktor MainWindow";

    setUi();
}

MainWindow::~MainWindow()
{
}

TaskView* MainWindow::getView()
{
    return m_tViewTree;
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

