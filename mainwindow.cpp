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
}

MainWindow::~MainWindow()
{
}

void MainWindow::addTask(Task* t)
{
    QTreeWidgetItem* item = 0;
    QList<QTreeWidgetItem*> result;
    QTreeWidgetItem* parentItem;

    if (t->hasParent()) {
        parentItem = findItem(t->getParent()->getTaskName());

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

QTreeWidgetItem* MainWindow::findItem(const QString& tName)
{
    // Szukanie w top level items
    // Oszczedza kilku przebiegow petli
    QList<QTreeWidgetItem*> topEntity;
    topEntity = m_taskView->findItems(tName, Qt::MatchExactly, TASK_N_C);
    if (!topEntity.isEmpty()) // udalo sie, konczymy.
        return topEntity[0];
    
    // Szukanie w pod-zadaniach
    QTreeWidgetItem* top;
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

void MainWindow::newActiveTask(Task* t)
{
    QTreeWidgetItem* result = findItem(t);
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
    QTreeWidgetItem* result = findItem(task);
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
        emit newTaskName(m_editedTaskName, item->text(TASK_N_C));
        m_editorActive = false;
    }
}

void MainWindow::setUi()
{
    setWindowTitle("TimeDevelop 0.1v");
    this->resize(800,600);

    QWidget* cw = new QWidget;
    setCentralWidget(cw);

    mainLayout = new QVBoxLayout;
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

    m_trayIcon = new QSystemTrayIcon(QIcon("icon/icon64.png"));
    m_trayIcon->show();

    connect(m_zakoncz, SIGNAL(clicked()), this, SIGNAL(zakonczClicked()));
    connect(m_wczytaj, SIGNAL(clicked()), this, SIGNAL(wczytajClicked()));
    connect(m_zapisz,  SIGNAL(clicked()), this, SIGNAL(zapiszClicked()));
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(m_taskView,SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
            this, SLOT(editTaskName(QTreeWidgetItem*, int)));
    connect(m_taskView,SIGNAL(itemChanged(QTreeWidgetItem*, int)),
            this, SLOT(endEditTaskName(QTreeWidgetItem*, int)));
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

