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
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QMessageBox>
#include <QInputDialog>

#include <QxtGui/QxtWindowSystem>

#include "mainwindow.h"
#include "windowattr.h"
#include "task.h"
#include "tasklistreader.h"
#include "finddialog.h"
#include "tasktreewidget.h"

MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent)
{
    qDebug() << "Konstruktor MainWindow" << Q_FUNC_INFO;

    setUi();
    setToolbar();
    setConnections();
}

MainWindow::~MainWindow()
{
}

TaskTreeWidget* MainWindow::getMainWidget()
{
    return m_taskTreeWidget;
}

void MainWindow::setUi()
{
    setWindowTitle("TimeDevelop 0.1v");
    this->resize(800,450);

    QWidget* cw = new QWidget;
    setCentralWidget(cw);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    cw->setLayout(mainLayout);

    m_taskTreeWidget = new TaskTreeWidget(this);
    QStringList labels;
    labels <<""<<""<<""<<""<<""<<""; // :)
    labels[TaskTreeWidget::ACTIVE_C] = QString("");
    labels[TaskTreeWidget::TASK_N_C] = QString(tr("Task name"));
    labels[TaskTreeWidget::APP_N_C] = QString(tr("Application name"));
    labels[TaskTreeWidget::ELAPS_C] = QString(tr("Elapsed time"));
    labels[TaskTreeWidget::PID_C] = QString("Pid");
    labels[TaskTreeWidget::WID_C] = QString("WId");
    
    m_taskTreeWidget->setHeaderLabels(labels);
    m_taskTreeWidget->setColumnWidth(TaskTreeWidget::ACTIVE_C, 40);
    m_taskTreeWidget->setColumnWidth(TaskTreeWidget::TASK_N_C, 350);
    m_taskTreeWidget->setColumnWidth(TaskTreeWidget::APP_N_C, 160);
    m_taskTreeWidget->hideColumn(TaskTreeWidget::PID_C);
    m_taskTreeWidget->hideColumn(TaskTreeWidget::WID_C);
    m_taskTreeWidget->setExpandsOnDoubleClick(true);
    m_taskTreeWidget->setSortingEnabled(true);
    m_taskTreeWidget->setAlternatingRowColors(true);

    m_taskTreeWidget->setDragEnabled(true);
    m_taskTreeWidget->setDragDropMode(QAbstractItemView::InternalMove);
    m_taskTreeWidget->setDropIndicatorShown(true);

    m_taskTreeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mainLayout->addWidget(m_taskTreeWidget);

    m_trayIcon = new QSystemTrayIcon(QIcon("icon/icon64.png"));
    m_trayIcon->show();

    statusBar()->showMessage(tr("Gotowy"));
}

void MainWindow::setToolbar()
{

m_newTaskAction = new QAction(QIcon("icon/add_task.png"),tr("&New Task"),this);
m_newTaskAction->setStatusTip(tr("Nowe zadanie"));
m_newTaskAction->setToolTip(tr("Dodaj nowe zadanie"));

m_deleteTaskAction=new QAction(QIcon("icon/delete_task.png"),tr("&Delete"),this);
m_deleteTaskAction->setStatusTip(tr("Usun zadanie"));

m_findTaskAction= new QAction(QIcon("icon/find_task.png"),tr("&Find Task"),this);
m_findTaskAction->setStatusTip(tr("Szukaj zadania"));

m_editTaskAction= new QAction(QIcon("icon/edit_task.png"),tr("&Edit Task"),this);
m_editTaskAction->setStatusTip(tr("Edytuj zadanie"));

m_perferencesAction=new QAction(QIcon("icon/preferences.png"),
tr("&Preferences"), this);
m_perferencesAction->setStatusTip(tr("Ustawienia"));

m_quitAction = new QAction(QIcon("icon/quit.png"), tr("Quit"), this);
m_quitAction->setStatusTip(tr("Zakoncz"));

    QToolBar* toolbarMain = new QToolBar(tr("Tools"), this);
    toolbarMain->setIconSize(QSize(48, 48));
    toolbarMain->addAction(m_newTaskAction);
    toolbarMain->addAction(m_deleteTaskAction);
    toolbarMain->addAction(m_editTaskAction);
    toolbarMain->addAction(m_findTaskAction);
    toolbarMain->addAction(m_perferencesAction);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QToolBar* toolbarQuit = new QToolBar(tr("Quit"), this);
    toolbarQuit->setIconSize(QSize(48,48));
    toolbarQuit->addWidget(spacer);
    toolbarQuit->addAction(m_quitAction);

    addToolBar(Qt::TopToolBarArea, toolbarMain);
    addToolBar(Qt::TopToolBarArea, toolbarQuit);

    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void MainWindow::setConnections()
{
    connect(m_newTaskAction,   SIGNAL(triggered()),
            m_taskTreeWidget, SLOT(processNewTask())); 
    connect(m_deleteTaskAction,SIGNAL(triggered()),
            m_taskTreeWidget, SLOT(processRemoveTask()));
    connect(m_editTaskAction,  SIGNAL(triggered()),
            m_taskTreeWidget, SLOT(processEditTaskName()));
    connect(m_findTaskAction,  SIGNAL(triggered()),
            m_taskTreeWidget, SLOT(processFindTask()));
    connect(m_perferencesAction,SIGNAL(triggered()),this,SIGNAL(orderPreferences()));
    connect(m_quitAction, SIGNAL(triggered()), this, SIGNAL(orderQuit()));


    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Trigger) {
        if (this->isHidden())
            this->show();
        else
            this->hide();
    }
}

