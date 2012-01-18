#ifndef TASK_VIEW_H
#define TASK_VIEW_H

#include <QTreeWidget>
#include <QString>

class Task;

class TaskView : public QTreeWidget {

    Q_OBJECT

    public:
        TaskView(QWidget* parent = 0);
        ~TaskView();

    public slots:
        void addTaskView(Task* t);
        void newActiveTask(Task* t);

    private:
        int countTaskView;
        int lastActiveTaskIndex;
};

#endif // TASK_VIEW_H

