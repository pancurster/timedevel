#ifndef FIND_DIALOG_H
#define FIND_DIALOG_H

#include <QDialog>

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

    private slots:
        void enableFindButton(const QString&);
        void findTask();

    private:
        void fillComboBox();

        QTreeWidget* treeWidget;

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
