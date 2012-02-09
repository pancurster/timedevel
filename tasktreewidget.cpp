// tasktreewidget.cpp
#include <QDebug>
#include <QWidget>
#include <QString>
#include <QWidget>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include <QDropEvent>
//#include <QMainWindow>
#include <QStatusBar>
#include <QInputDialog>

#include <QxtGui/QxtWindowSystem>

#include "task.h"
#include "windowattr.h"
#include "finddialog.h"
#include "tasktreewidget.h"
#include "mainwindow.h"

TaskTreeWidget::TaskTreeWidget(QWidget* parent) :
    QTreeWidget(parent),
    m_activeItem(0)
{
}

TaskTreeWidget::~TaskTreeWidget()
{
}

void TaskTreeWidget::addTask(Task* t)
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
        item = new QTreeWidgetItem(this);
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
        this->insertTopLevelItem(this->topLevelItemCount(), item);
    }

    qDebug() << Q_FUNC_INFO << "Dodano zadanie do widoku";
}

void TaskTreeWidget::newActiveTask(Task* t)
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

        getMainWindow()->statusBar()->showMessage(
            QString(tr("Poprzednie aktywne zadanie: %1"))
            .arg(m_activeItem->text(TASK_N_C)));
    }

    font = result->font(TASK_N_C);
    font.setBold(true);
    result->setFont(TASK_N_C, font);
    m_activeItem = result;
}

void TaskTreeWidget::refreshElapsedTime(const QString& task, int newElapsedTime)
{
    qDebug() << Q_FUNC_INFO << task <<" "<<newElapsedTime;
    QTreeWidgetItem* result = findItem(task);
    if (!result) {
        qDebug() << "   BLAD: nie znaleziono zadania" << task << Q_FUNC_INFO;
        return;
    }
    result->setText(ELAPS_C, toMinSec(newElapsedTime));
}

void TaskTreeWidget::removeTaskFromView(const QString& taskName)
{
    QTreeWidgetItem* toRemove;
    QTreeWidgetItem* removed;
    int index = -1;
    toRemove = findTopLevelItem(taskName);
    if (toRemove) {
        index   = this->indexOfTopLevelItem(toRemove);
        removed = this->takeTopLevelItem(index);
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

void TaskTreeWidget::dropEvent(QDropEvent* event)
{
    QList<QTreeWidgetItem*> selectList;
    selectList = selectedItems();
    QString taskName = selectList.at(0)->text(TASK_N_C);
    QString newParentName;

    QTreeWidgetItem* newParentItem = itemAt(event->pos());
    if (newParentItem) {
        newParentName = newParentItem->text(TASK_N_C);
    } else {
        newParentName = "No parent";
    }
    qDebug() << "Moved Task: "<<taskName<<" his new Parent is: "
             <<newParentName;

    emit orderReparent(taskName, newParentName);

    QTreeWidget::dropEvent(event);
}

void TaskTreeWidget::processNewTask()
{
    bool ok;
    QString newTask = QInputDialog::getText(this, tr("Dodaj nowe zadanie"),
                      tr("Nazwa zadania:"), QLineEdit::Normal,
                      tr("Nowe Zadanie"), &ok);
    if ( ok && !newTask.isEmpty()) {
        emit orderNewTask(newTask);
    }
}

void TaskTreeWidget::processRemoveTask()
{
    QList<QTreeWidgetItem*> itemToRemove = this->selectedItems();
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

void TaskTreeWidget::processEditTaskName()
{
    bool ok = false;
    QString newName("");
    QList<QTreeWidgetItem*> itemToRename = this->selectedItems();

    if (!itemToRename.isEmpty()) {
        newName = QInputDialog::getText(this, tr("Zmiana nazwy zadania"),
                  tr("Nowa nazwa: "), QLineEdit::Normal,
                  QString("%1").arg(itemToRename[0]->text(TASK_N_C)), &ok);

        if (!newName.isEmpty() && newName != itemToRename[0]->text(TASK_N_C)) {
            emit orderEditTaskName(itemToRename[0]->text(TASK_N_C), newName);
// To nie powinno tak wygladac. Model --> Widok (taskNameChanged->NewTaskName)
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

void TaskTreeWidget::processFindTask()
{
    FindTaskDialog* find = new FindTaskDialog(this);

    //fix. Dodano wylaczanie modulu detekcji focusa poniewaz powodowal on
    //bledne dzialanie comboboxa w okienku szukania.
    emit offFocusDetector();
    find->exec();
    emit onFocusDetector();

    // Odznaczanie jakiegokolwiek itema, jesli zaznaczony
    this->setCurrentItem(0);
    // Potrzebne multi zaznaczanie. Wylaczane nizej.
    this->setSelectionMode(QAbstractItemView::MultiSelection);

    QList<QListWidgetItem*> list = find->getReturnValue();
    int i;
    for (i = 0; i < list.size(); ++i) {
        QTreeWidgetItem* findedItem = findItem(list[i]->text());
        if (findedItem)
            this->setCurrentItem(findedItem);
    }

    this->setSelectionMode(QAbstractItemView::ExtendedSelection);

    getMainWindow()->statusBar()->showMessage(
            QString(tr("Zaznaczono %1 zadania")).arg(i));
}

QTreeWidgetItem* TaskTreeWidget::findItem(const QString& tName)
{
    // Szukanie w top level items
    // Oszczedza kilku przebiegow petli
    QTreeWidgetItem* top = findTopLevelItem(tName);
    if (top)
        return top; //udalo sie, konczymy
    
    // Szukanie w pod-zadaniach
    QTreeWidgetItem* child;
    for (int t = 0; t < this->topLevelItemCount(); ++t) {
        top = this->topLevelItem(t);
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

QTreeWidgetItem* TaskTreeWidget::findItem(Task* t)
{
    return findItem(t->getTaskName());
}

QTreeWidgetItem* TaskTreeWidget::findTopLevelItem(const QString& tName)
{
    QList<QTreeWidgetItem*> topEntity;
    topEntity = this->findItems(tName, Qt::MatchExactly, TASK_N_C);
    if (!topEntity.isEmpty())
        return topEntity[0];
    else
        return 0;
}

QMainWindow* TaskTreeWidget::getMainWindow()
{
    return (QMainWindow*)QTreeWidget::parentWidget()->parentWidget();
}

void TaskTreeWidget::InfoMessageBox(const QString& windowTitle,const QString& info)
{
        QMessageBox infoBox;
        infoBox.setWindowTitle(windowTitle);
        infoBox.setText(info);
        infoBox.setIcon(QMessageBox::Information);
        infoBox.exec();
}

QString TaskTreeWidget::toMinSec(int wholeTime, const QString& spliter)
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

