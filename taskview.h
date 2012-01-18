#ifndef TASK_VIEW_H
#define TASK_VIEW_H

#include <QTreeWidget>
class Task;

class TaskView : public QTreeWidget {
    public:
        TaskView(QWidget* parent = 0);
        ~TaskView();
        void addTask(Task* t);
        void addTaskList(QMap<int, Task*>* tl);
    private:
};

#endif // TASK_VIEW_H

