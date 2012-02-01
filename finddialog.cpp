#include <QDebug>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QList>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "finddialog.h"

FindTaskDialog::FindTaskDialog(QTreeWidget* t, QWidget* parent) :
    QDialog(parent), treeWidget(t)
{
    m_labelTaskName = new QLabel(tr("&Szukaj zadania:"));
    m_editTaskName = new QLineEdit;
    m_labelTaskName->setBuddy(m_editTaskName);

    m_labelAppName = new QLabel(tr("Nazwa aplikacji"));
    m_comboAppName = new QComboBox;
    fillComboBox();

    m_buttonFind = new QPushButton(tr("Znajdz"));
    m_buttonFind->setDefault(true);
    m_buttonFind->setEnabled(false);

    m_buttonSelect = new QPushButton(tr("Zaznacz\n i\n Zamknij"));
    m_buttonSelect->setEnabled(false);

    m_listTask = new QListWidget;

    m_buttonClose = new QPushButton(tr("Anuluj"));

    QHBoxLayout* layout_taskname = new QHBoxLayout;
    layout_taskname->addWidget(m_labelTaskName);
    layout_taskname->addWidget(m_editTaskName);

    QHBoxLayout* layout_appname = new QHBoxLayout;
    layout_appname->addWidget(m_labelAppName);
    layout_appname->addWidget(m_comboAppName);

    QVBoxLayout* layout_left = new QVBoxLayout;
    layout_left->addLayout(layout_taskname);
    layout_left->addLayout(layout_appname);
    layout_left->addWidget(m_listTask);

    QVBoxLayout* layout_right = new QVBoxLayout;
    layout_right->addWidget(m_buttonFind);
    layout_right->addWidget(m_buttonClose);
    layout_right->addStretch();
    layout_right->addWidget(m_buttonSelect);
    layout_right->addStretch();

    QHBoxLayout* m_mainLayout = new QHBoxLayout;
    m_mainLayout->addLayout(layout_left);
    m_mainLayout->addLayout(layout_right);
    setLayout(m_mainLayout);
    
    resize(700, 350);
    setWindowTitle(tr("Szukaj zadania..."));

    connect(m_editTaskName, SIGNAL(textChanged(const QString&)),
            this, SLOT(enableFindButton(const QString&)));
    connect(m_buttonFind, SIGNAL(clicked()), this, SLOT(findTask()));
    connect(m_buttonClose, SIGNAL(clicked()), this, SLOT(close()));

}

FindTaskDialog::~FindTaskDialog()
{
    delete m_labelTaskName;
    delete m_editTaskName;
    delete m_labelAppName;
    delete m_comboAppName;
    delete m_buttonFind;
    delete m_buttonClose;
    delete m_buttonSelect;
    delete m_listTask;
    delete m_mainLayout;
}

void FindTaskDialog::enableFindButton(const QString&)
{
    if (!m_buttonFind->isEnabled())
        m_buttonFind->setEnabled(true);
    return;
}

void FindTaskDialog::findTask()
{
    qDebug() << "NACISNIETO FIND";
    m_listTask->clear();
    m_buttonFind->setEnabled(false);
    QString querry = m_editTaskName->text();
    qDebug() << "querry: " << querry;

    bool finded = false;
    QTreeWidgetItem* top;
    QTreeWidgetItem* child;
    for (int t = 0; t < treeWidget->topLevelItemCount(); ++t) {
        top = treeWidget->topLevelItem(t);
        if (top->text(1).contains(querry, Qt::CaseInsensitive)) {
            m_listTask->addItem(top->text(1));
            finded = true;
        }

        if (top->childCount() > 0) {
            for (int c = 0; c < top->childCount(); ++c) {
                child = top->child(c);
                if (child->text(1).contains(querry, Qt::CaseInsensitive)) {
                    m_listTask->addItem(child->text(1));
                    finded = true;
                }
            }
        }
    }

    if (!finded)
        m_listTask->addItem(tr("Nic nie znaleziono"));
}

void FindTaskDialog::fillComboBox()
{
    qDebug()<< Q_FUNC_INFO;
    QList<QTreeWidgetItem*> listOfTask;
    for (int i = treeWidget->topLevelItemCount(); i > 0; --i) {
        listOfTask.append(treeWidget->topLevelItem(i));
    }
    qDebug()<< "End filling listOfTask";

    QList<QString> listOfAppNames;
    for (int i = listOfTask.size()-1; i > 0; --i) {
        listOfAppNames.append(listOfTask[i]->text(2));
    }
    qDebug()<< "End filling listOfAppNames";
    m_comboAppName->addItem(tr("Dowolna..."));
    m_comboAppName->addItems(listOfAppNames);
}

