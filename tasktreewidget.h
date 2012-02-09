#ifndef TASK_TREE_WIDGET_H
#define TASK_TREE_WIDGET_H

#include <QTreeWidget>

class Task;
class QString;
class QWidget;
class QTreeWidgetItem;
class QMainWindow;

class TaskTreeWidget : public QTreeWidget {

    Q_OBJECT

    public:
        TaskTreeWidget(QWidget* parent);
        ~TaskTreeWidget();

        // Kolejnosc pol w wyliczeniu wyznacza kolejnosc w m_taskView typu
        // QTreeWidget. Zabezpiecza przed rzaglerka cygerkami w funkcjach
        // ktore oczekuja numeru kolumny widgetu QTreeWidget.
        static enum {ACTIVE_C, TASK_N_C, APP_N_C, ELAPS_C,
              PID_C, WID_C} columnOrder;

    public slots:
        void addTask(Task* t);
        void newActiveTask(Task* t);
        void refreshElapsedTime(const QString& task, int newElapsedTime);
        void removeTaskFromView(const QString& taskName);

        void processRemoveTask();   // emits orderRemoveTask(...)
        void processNewTask();
        void processEditTaskName();
        void processFindTask();

    signals:
        void orderFindTask(const QString& taskName);
        void orderNewTask(const QString& taskName);
        void orderEditTaskName(const QString& taskName, const QString& newName);
        void orderRemoveTask(const QString& taskName);
        void orderReparent(const QString& taskName, const QString& parent);

        void offFocusDetector();
        void onFocusDetector();

    protected:
        virtual bool dropMimeData(QTreeWidgetItem* parent, int index,
                          const QMimeData* data, Qt::DropAction action);
    private:
        QTreeWidgetItem* findItem(const QString& tName);
        QTreeWidgetItem* findItem(Task* t);
        QTreeWidgetItem* findTopLevelItem(const QString& taskName);

        /// Zwraca poprawny wskaznik na QMainWindow
        QMainWindow* getMainWindow();

        QString toMinSec(int counter, const QString& spliter = ":");
        void    InfoMessageBox(const QString& windowTitle,const QString& info);

        QTreeWidgetItem* m_activeItem;      //Aktualnie aktywne zadanie
};

#endif // TASK_TREE_WIDGET_H
