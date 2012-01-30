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
#include <QSystemTrayIcon>
#include <QIcon>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QMessageBox>

#include <QxtGui/QxtWindowSystem>

#include "mainwindow.h"
#include "windowattr.h"
#include "task.h"
#include "tasklistreader.h"

MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent),
    m_activeItem(0),
    m_editedTaskName(""),
    m_editorActive(false)
{
    qDebug() << "Konstruktor MainWindow";

    setUi();
    setToolbar();
    setConnections();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setUi()
{
    setWindowTitle("TimeDevelop 0.1v");
    this->resize(800,450);

    QWidget* cw = new QWidget;
    setCentralWidget(cw);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    cw->setLayout(mainLayout);

    m_taskView= new QTreeWidget;
    QStringList labels;
    labels <<""<<""<<""<<""<<""<<""; // :)
    labels[ACTIVE_C] = QString("");
    labels[TASK_N_C] = QString("Task name");
    labels[APP_N_C] = QString("Application name");
    labels[ELAPS_C] = QString("Elapsed time");
    labels[PID_C] = QString("Pid");
    labels[WID_C] = QString("WId");
    m_taskView->setHeaderLabels(labels);
    m_taskView->setColumnWidth(ACTIVE_C, 40);
    m_taskView->setColumnWidth(TASK_N_C, 60);
    mainLayout->addWidget(m_taskView);

    m_trayIcon = new QSystemTrayIcon(QIcon("icon/icon64.png"));
    m_trayIcon->show();

    statusBar()->showMessage("Ready");
}

void MainWindow::setToolbar()
{
    m_newTaskAction = new QAction(QIcon("icon/add_task.png"),"&New Task",this);
    m_newTaskAction->setStatusTip("Nowe zadanie");
    m_deleteTaskAction=new QAction(QIcon("icon/delete_task.png"),"&Delete",this);
    m_deleteTaskAction->setStatusTip("Usun zadanie");
    m_findTaskAction= new QAction(QIcon("icon/find_task.png"),"&Find Task",this);
    m_findTaskAction->setStatusTip("Szukaj zadania");
    m_editTaskAction= new QAction(QIcon("icon/edit_task.png"),"&Edit Task",this);
    m_editTaskAction->setStatusTip("Edytuj zadanie");
    m_perferencesAction=new QAction(QIcon("icon/preferences.png"),"&Preferences",this);
    m_perferencesAction->setStatusTip("Ustawienia");
    m_quitAction = new QAction(QIcon("icon/quit.png"), "Quit", this);
    m_quitAction->setStatusTip("Zakoncz");

    m_toolbar = new QToolBar("Tools", this);
    m_toolbar->setIconSize(QSize(48, 48));
    m_toolbar->addAction(m_newTaskAction);
    m_toolbar->addAction(m_deleteTaskAction);
    m_toolbar->addAction(m_editTaskAction);
    m_toolbar->addAction(m_findTaskAction);
    m_toolbar->addAction(m_perferencesAction);

    QToolBar* toolbarQuit = new QToolBar("Quit", this);
    toolbarQuit->setIconSize(QSize(48,48));
    toolbarQuit->addAction(m_quitAction);

    addToolBar(Qt::RightToolBarArea, m_toolbar);
    addToolBar(Qt::RightToolBarArea, toolbarQuit);
}

void MainWindow::setConnections()
{
connect(m_newTaskAction,   SIGNAL(triggered()),this,SIGNAL(orderNewTask())); 
connect(m_deleteTaskAction,SIGNAL(triggered()),
        this, SLOT(processRemoveTask()));
connect(m_findTaskAction,  SIGNAL(triggered()),this,SIGNAL(orderFindTask()));
connect(m_editTaskAction,  SIGNAL(triggered()),this,SIGNAL(orderEditTaskName()));
connect(m_perferencesAction,SIGNAL(triggered()),this,SIGNAL(orderPreferences()));
connect(m_quitAction, SIGNAL(triggered()), this, SIGNAL(orderQuit()));

    connect(m_taskView,SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
            this, SLOT(editTaskName(QTreeWidgetItem*, int)));
    connect(m_taskView,SIGNAL(itemChanged(QTreeWidgetItem*, int)),
            this, SLOT(endEditTaskName(QTreeWidgetItem*, int)));

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::removeTaskFromView(const QString& taskName)
{
    QTreeWidgetItem* toRemove;
    QTreeWidgetItem* removed;
    int index = -1;
    toRemove = findTopLevelTask(taskName);
    if (toRemove) {
        index   = m_taskView->indexOfTopLevelItem(toRemove);
        removed = m_taskView->takeTopLevelItem(index);
        delete removed;
        return;
    }

    toRemove = findTask(taskName);
    if (toRemove) {
        toRemove->parent()->removeChild(toRemove);
        delete toRemove;
        return;
    }

    Q_ASSERT(!"Nie znalezino zadania do usuniecia!?");
}

void MainWindow::processRemoveTask()
{
//    QTreeWidgetItem* itemToRemove = m_taskView->currentItem();
    QList<QTreeWidgetItem*> itemToRemove = m_taskView->selectedItems();
    if (!itemToRemove.isEmpty()) {
        QMessageBox removeBox;
        removeBox.setText(QString("Napewno chcesz usunac zadanie:<b> %1</b>")
                          .arg(itemToRemove[0]->text(TASK_N_C)));
        removeBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        removeBox.setDefaultButton(QMessageBox::Cancel);
        int ret = removeBox.exec();
        if (ret == QMessageBox::Yes)
            emit orderRemoveTask(itemToRemove[0]->text(TASK_N_C));
    } else {
        QMessageBox noTaskSelected;
        noTaskSelected.setText("Nie zaznaczono zadania do usuniecia");
        noTaskSelected.exec();
    }
    return;
}

void MainWindow::addTask(Task* t)
{
    QTreeWidgetItem* item = 0;
    QList<QTreeWidgetItem*> result;
    QTreeWidgetItem* parentItem;

    if (t->hasParent()) {
        parentItem = findTask(t->getParent()->getTaskName());

        if (parentItem) {
            item = new QTreeWidgetItem(parentItem);
        } else {
            qDebug()<<"   BLAD: Nie znaleziono parentItem!";
            return;
        }

    } else {
        item = new QTreeWidgetItem(m_taskView);
    }

    QString time = toMinSec(t->getElapsedTime());
    item->setText(TASK_N_C, t->getTaskName());
    item->setText(ELAPS_C, time);
    item->setText(APP_N_C, t->getAppName());

    if (t->hasWAttr()) {
        const WindowAttr* wa = t->getWAttr();
        item->setText(PID_C, QString::number(wa->getPid()));
        item->setText(WID_C, QString::number(wa->getWId()));
    }

    if (t->hasParent() && parentItem) {
        parentItem->addChild(item);
    } else {
        item->setChildIndicatorPolicy(
                        QTreeWidgetItem::DontShowIndicatorWhenChildless);
        m_taskView->insertTopLevelItem(m_taskView->topLevelItemCount(), item);
        m_taskView->resizeColumnToContents(1);
    }

    qDebug() << "MainWindow::addTask: dodano zadanie do widoku";
}

QTreeWidgetItem* MainWindow::findTopLevelTask(const QString& tName)
{
    QList<QTreeWidgetItem*> topEntity;
    topEntity = m_taskView->findItems(tName, Qt::MatchExactly, TASK_N_C);
    if (!topEntity.isEmpty())
        return topEntity[0];
    else
        return 0;
}

QTreeWidgetItem* MainWindow::findTask(const QString& tName)
{
    // Szukanie w top level items
    // Oszczedza kilku przebiegow petli
    QTreeWidgetItem* top = findTopLevelTask(tName);
    if (top)
        return top; //udalo sie, konczymy
    
    // Szukanie w pod-zadaniach
    QTreeWidgetItem* child;
    for (int t = 0; t < m_taskView->topLevelItemCount(); ++t) {
        top = m_taskView->topLevelItem(t);
        if (top->childCount() > 0) {
            for (int c = 0; c < top->childCount(); ++c) {
                child = top->child(c);
                if (tName == child->text(TASK_N_C))
                    return child;
            }
        }
    }
    return 0;
}

QTreeWidgetItem* MainWindow::findTask(Task* t)
{
    return findTask(t->getTaskName());
}

void MainWindow::newActiveTask(Task* t)
{
    QTreeWidgetItem* result = findTask(t);
    if (!result) {
        qDebug() <<"newActiveTask: nie znaleziono zadania "<<t->getTaskName();
        return;
    }

    QFont font;
    if (m_activeItem) {
        font = m_activeItem->font(TASK_N_C);
        font.setBold(false);
        m_activeItem->setFont(TASK_N_C, font);
    }

    font = result->font(TASK_N_C);
    font.setBold(true);
    result->setFont(TASK_N_C, font);
    m_activeItem = result;
}

void MainWindow::refreshElapsedTime(const QString& task, int newElapsedTime)
{
    qDebug() << "Wywolanie refreshElapsedTime" << task <<" "<<newElapsedTime;
    QTreeWidgetItem* result = findTask(task);
    if (!result) {
        qDebug() << "   BLAD: refreshElapsedTime, nie znaleziono zadania"
                 << task;
        return;
    }
    result->setText(ELAPS_C, toMinSec(newElapsedTime));
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

void MainWindow::editTaskName(QTreeWidgetItem* item, int /*column*/)
{
    // potrzebne do updejtu w modelu (nazwa zad. - klucz)
    m_editedTaskName = item->text(TASK_N_C);
    m_taskView->openPersistentEditor(item, TASK_N_C);
    m_editorActive = true;
}

void MainWindow::endEditTaskName(QTreeWidgetItem* item, int /*column*/)
{
    // Slot/funkcja ta, polaczona jest z sygnalem itemChanged klasy typu
    // QTreeWidget i wywoływana powinna być w przypadku kiedy zadanie edytowane
    // jest przez urzytkownika. Zmienna m_editorActive wprowadzona poniewaz
    // przy odczytywaniu przy starcie wczesniej zapisanych zadan wypełniana
    // jest składowa m_taskView co powoduje emitowanie sygnału i wywołanie tej
    // funkcji - prowadzac do błedu.
    if (m_editorActive == true ) {
        m_taskView->closePersistentEditor(item, TASK_N_C);
        emit orderEditTaskName(m_editedTaskName, item->text(TASK_N_C));
        m_editorActive = false;
    }
}


QString MainWindow::toMinSec(int wholeTime, const QString spliter)
{
    wholeTime /= 1000;
    int hour= wholeTime / 3600;
    wholeTime %= 3600;
    int sec = wholeTime % 60;
    int min = wholeTime / 60;
    QString time = QString("%1%2%3%4%5")
        .arg(hour).arg(spliter)
        .arg(min, 2, 10, QChar('0')).arg(spliter)
        .arg(sec, 2, 10, QChar('0'));

    return time;
}

