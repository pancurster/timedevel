#ifndef FIND_DIALOG_H
#define FIND_DIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <QList>

class QTreeWidget;
class QLabel;
class QPushButton;
class QLineEdit;
class QComboBox;
class QListWidget;
class QHBoxLayout;

class FindTaskDialog : public QDialog {

    Q_OBJECT

    public:
        FindTaskDialog(QTreeWidget* t, QWidget* parent = 0);
        ~FindTaskDialog();

        QList<QListWidgetItem*> getReturnValue();

    private slots:
        void enableFindButton();
        void findTask();
        void selectItemsAndDone();

    private:
        void fillComboBox();

        QTreeWidget* treeWidget;
        QList<QListWidgetItem*> retValue;

        QLabel*      m_labelTaskName;
        QLabel*      m_labelAppName;
        QPushButton* m_buttonFind;
        QPushButton* m_buttonClose;
        QPushButton* m_buttonSelect;
        QLineEdit*   m_editTaskName;
        QComboBox*   m_comboAppName;
        QListWidget* m_listTask;
        QHBoxLayout* m_mainLayout;

};

#endif // FIND_DIALOG_H
