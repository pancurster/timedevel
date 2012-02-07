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
#include <QInputDialog>

#include <QxtGui/QxtWindowSystem>

#include "mainwindow.h"
#include "windowattr.h"
#include "task.h"
#include "tasklistreader.h"
#include "finddialog.h"

MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent),
    m_activeItem(0)
{
    qDebug() << "Konstruktor MainWindow" << Q_FUNC_INFO;

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
    labels[TASK_N_C] = QString(tr("Task name"));
    labels[APP_N_C] = QString(tr("Application name"));
    labels[ELAPS_C] = QString(tr("Elapsed time"));
    labels[PID_C] = QString("Pid");
    labels[WID_C] = QString("WId");
    m_taskView->setHeaderLabels(labels);
    m_taskView->setColumnWidth(ACTIVE_C, 40);
    m_taskView->setColumnWidth(TASK_N_C, 350);
    m_taskView->setColumnWidth(APP_N_C, 160);
    m_taskView->hideColumn(PID_C);
    m_taskView->hideColumn(WID_C);
    m_taskView->setExpandsOnDoubleClick(true);
    m_taskView->setSortingEnabled(true);
    m_taskView->setAlternatingRowColors(true);

    m_taskView->setDragEnabled(true);
    m_taskView->setDragDropMode(QAbstractItemView::InternalMove);
    m_taskView->setDropIndicatorShown(true);

    m_taskView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    mainLayout->addWidget(m_taskView);

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
            this, SLOT(processNewTask())); 
    connect(m_deleteTaskAction,SIGNAL(triggered()),
            this, SLOT(processRemoveTask()));
    connect(m_editTaskAction,  SIGNAL(triggered()),
            this, SLOT(processEditTaskName()));
    connect(m_findTaskAction,  SIGNAL(triggered()),
            this, SLOT(processFindTask()));
    connect(m_perferencesAction,SIGNAL(triggered()),this,SIGNAL(orderPreferences()));
    connect(m_quitAction, SIGNAL(triggered()), this, SIGNAL(orderQuit()));


    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::processNewTask()
{
    bool ok;
    QString newTask = QInputDialog::getText(this, tr("Dodaj nowe zadanie"),
                      tr("Nazwa zadania:"), QLineEdit::Normal,
                      tr("Nowe Zadanie"), &ok);
    if ( ok && !newTask.isEmpty()) {
        emit orderNewTask(newTask);
    }
}

void MainWindow::removeTaskFromView(const QString& taskName)
{
    QTreeWidgetItem* toRemove;
    QTreeWidgetItem* removed;
    int index = -1;
    toRemove = findTopLevelItem(taskName);
    if (toRemove) {
        index   = m_taskView->indexOfTopLevelItem(toRemove);
        removed = m_taskView->takeTopLevelItem(index);
        delete removed;
        return;
    }

    toRemove = findItem(taskName);
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
        removeBox.setText(QString(tr("Napewno chcesz usunac"
                "zadanie:<b> %1</b>")).arg(itemToRemove[0]->text(TASK_N_C)));
        removeBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        removeBox.setWindowTitle(tr("Usun zadanie..."));
        removeBox.setIcon(QMessageBox::Question);
        removeBox.setDefaultButton(QMessageBox::Cancel);
        int ret = removeBox.exec();
        if (ret == QMessageBox::Yes)
            emit orderRemoveTask(itemToRemove[0]->text(TASK_N_C));
    } else {
        InfoMessageBox(tr("Zaznacz zadanie..."),
                       tr("Nie zaznaczono zadania do usniecia"));
    }
    return;
}

void MainWindow::InfoMessageBox(const QString& windowTitle,const QString& info)
{
        QMessageBox infoBox;
        infoBox.setWindowTitle(windowTitle);
        infoBox.setText(info);
        infoBox.setIcon(QMessageBox::Information);
        infoBox.exec();
}

void MainWindow::processEditTaskName()
{
    bool ok = false;
    QString newName("");
    QList<QTreeWidgetItem*> itemToRename = m_taskView->selectedItems();

    if (!itemToRename.isEmpty()) {
        newName = QInputDialog::getText(this, tr("Zmiana nazwy zadania"),
                  tr("Nowa nazwa: "), QLineEdit::Normal,
                  QString("%1").arg(itemToRename[0]->text(TASK_N_C)), &ok);

        if (!newName.isEmpty() && newName != itemToRename[0]->text(TASK_N_C)) {
            emit orderEditTaskName(itemToRename[0]->text(TASK_N_C), newName);
            itemToRename[0]->setText(TASK_N_C, newName);
        }
        return;

    } else {
        InfoMessageBox(tr("Zaznacz zadanie..."),
                       tr("Nie zaznaczono zadania do operacji zmiany nazwy"));
        return;
    }

    Q_ASSERT(!"Program nie mogl osiagnac tego punktu");
}

void MainWindow::addTask(Task* t)
{
    QTreeWidgetItem* item = 0;
    QList<QTreeWidgetItem*> result;
    QTreeWidgetItem* parentItem;

    // Dodawanie zadania jako POD-ZADANIA
    if (t->hasParent()) {
        parentItem = findItem(t->getParent()->getTaskName());

        if (parentItem) {
            item = new QTreeWidgetItem(parentItem);
        } else {
            qDebug()<< "   BLAD: Nie znaleziono parentItem!" << Q_FUNC_INFO;
            return;
        }

    // Dodawanie zadania jako ZADANIA-GLOWNEGO
    } else {
        item = new QTreeWidgetItem(m_taskView);
    }


    /* * * Ustawianie pol QTreeWidgetItem na podstawie Task* t * * */

    int time = t->getElapsedTime();
    item->setText(TASK_N_C, t->getTaskName());
    item->setText(ELAPS_C, toMinSec(time));
    item->setText(APP_N_C, t->getAppName());
    item->setIcon(ACTIVE_C, QIcon("icon/prior_not_known.png"));

    if (t->hasWAttr()) {
        const WindowAttr* wa = t->getWAttr();
        item->setText(PID_C, QString::number(wa->getPid()));
        item->setText(WID_C, QString::number(wa->getWId()));
    }

    if (t->hasParent() && parentItem) {
        parentItem->addChild(item);

        // Trzeba odswiezyc tez czas zadania-rodzica
        Task* parent = t->getParent();
        int time = parent->getElapsedTime() + parent->getElpasedChildrenTime();
        parentItem->setText(ELAPS_C, toMinSec(time));
    } else {
        item->setChildIndicatorPolicy(
                        QTreeWidgetItem::DontShowIndicatorWhenChildless);
        m_taskView->insertTopLevelItem(m_taskView->topLevelItemCount(), item);
    }

    qDebug() << Q_FUNC_INFO << "Dodano zadanie do widoku";
}

QTreeWidgetItem* MainWindow::findTopLevelItem(const QString& tName)
{
    QList<QTreeWidgetItem*> topEntity;
    topEntity = m_taskView->findItems(tName, Qt::MatchExactly, TASK_N_C);
    if (!topEntity.isEmpty())
        return topEntity[0];
    else
        return 0;
}

QTreeWidgetItem* MainWindow::findItem(const QString& tName)
{
    // Szukanie w top level items
    // Oszczedza kilku przebiegow petli
    QTreeWidgetItem* top = findTopLevelItem(tName);
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

QTreeWidgetItem* MainWindow::findItem(Task* t)
{
    return findItem(t->getTaskName());
}

void MainWindow::processFindTask()
{
    FindTaskDialog* find = new FindTaskDialog(m_taskView);

    //fix. Dodano wylaczanie modulu detekcji focusa poniewaz powodowal on
    //bledne dzialanie comboboxa w okienku szukania.
    emit offFocusDetector();
    find->exec();
    emit onFocusDetector();

    // Odznaczanie jakiegokolwiek itema, jesli zaznaczony
    m_taskView->setCurrentItem(0);
    // Potrzebne multi zaznaczanie. Wylaczane nizej.
    m_taskView->setSelectionMode(QAbstractItemView::MultiSelection);

    QList<QListWidgetItem*> list = find->getReturnValue();
    int i;
    for (i = 0; i < list.size(); ++i) {
        QTreeWidgetItem* findedItem = findItem(list[i]->text());
        if (findedItem)
            m_taskView->setCurrentItem(findedItem);
    }

    m_taskView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    statusBar()->showMessage(QString(tr("Zaznaczono %1 zadania")).arg(i));
}

void MainWindow::newActiveTask(Task* t)
{
    QTreeWidgetItem* result = findItem(t);
    if (!result) {
        qDebug() <<Q_FUNC_INFO << "Nie znaleziono zadania "<<t->getTaskName();
        return;
    }

    QFont font;
    if (m_activeItem) {
        font = m_activeItem->font(TASK_N_C);
        font.setBold(false);
        m_activeItem->setFont(TASK_N_C, font);
        statusBar()->showMessage(QString(tr("Poprzednie aktywne zadanie: %1"))
                                 .arg(m_activeItem->text(TASK_N_C)));
    }

    font = result->font(TASK_N_C);
    font.setBold(true);
    result->setFont(TASK_N_C, font);
    m_activeItem = result;
}

void MainWindow::refreshElapsedTime(const QString& task, int newElapsedTime)
{
    qDebug() << Q_FUNC_INFO << task <<" "<<newElapsedTime;
    QTreeWidgetItem* result = findItem(task);
    if (!result) {
        qDebug() << "   BLAD: nie znaleziono zadania" << task << Q_FUNC_INFO;
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

QString MainWindow::toMinSec(int wholeTime, const QString& spliter)
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

