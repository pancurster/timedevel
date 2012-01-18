//taskview.cpp
#include <QTreeWidgetItem>
#include <QStringList>
#include <QString>

#include "task.h"
#include "windowattr.h"
#include "taskview.h"

TaskView::TaskView(QWidget* parent):
    QTreeWidget(parent)
{
    QStringList labels;
    labels << "Task name" << "Pid" << "X Window Id" << "Time elapsed";
    setHeaderLabels(labels);

}

TaskView::~TaskView()
{
}

void TaskView::addTask(Task* t)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(this);
    item->setText(0, t->getTaskName());
    item->setText(3, QString::number(t->getElapsedTime()/1000));
    if (t->hasWAttr()) {
        const WindowAttr* wa = t->getWAttr();
        item->setText(1, QString::number(wa->getPid()));
        item->setText(2, QString::number(wa->getWId()));
    }
    insertTopLevelItem(topLevelItemCount()+1, item);
}

void TaskView::addTaskList(QMap<int, Task*>* tl)
{
}

