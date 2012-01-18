//taskview.cpp
#include <QDebug>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QString>

#include "task.h"
#include "windowattr.h"
#include "taskview.h"

TaskView::TaskView(QWidget* parent):
    QTreeWidget(parent),
    countTaskView(0), lastActiveTaskIndex(0)
{
    QStringList labels;
    labels << "A" <<"Task name" << "Pid" << "X Window Id" << "Time elapsed";
    setHeaderLabels(labels);

}

TaskView::~TaskView()
{
}

void TaskView::addTaskView(Task* t)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(this);
    item->setText(1, t->getTaskName());
    item->setText(4, QString::number(t->getElapsedTime()/1000));
    if (t->hasWAttr()) {
        const WindowAttr* wa = t->getWAttr();
        item->setText(2, QString::number(wa->getPid()));
        item->setText(3, QString::number(wa->getWId()));
    }
    insertTopLevelItem(countTaskView, item);
    ++countTaskView;

    qDebug() << "TaskView::addTask: dodano zadanie do widoku";
}

void TaskView::newActiveTask(Task* t)
{
    QList<QTreeWidgetItem* > match = findItems(t->getTaskName(),
                                               Qt::MatchExactly, 1);
    if (match.isEmpty())
        return;
    topLevelItem(lastActiveTaskIndex)->setText(0,"");
    match[0]->setText(0, ">");
    lastActiveTaskIndex = indexOfTopLevelItem(match[0]);
}

