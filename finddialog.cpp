#include <QDebug>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>

#include "finddialog.h"

FindTaskDialog::FindTaskDialog(QTreeWidget* t, QWidget* parent) :
    QDialog(parent), treeWidget(t)
{
    m_labelTaskName = new QLabel(tr("Szukaj zadania:"));
    m_editTaskName = new QLineEdit;
    m_labelTaskName->setBuddy(m_editTaskName);

    m_labelAppName = new QLabel(tr("Szukaj w:"));
    m_comboAppName = new QComboBox;
    fillComboBox();

    m_buttonFind = new QPushButton(tr("&Szukaj"));
    m_buttonFind->setDefault(true);
    m_buttonFind->setEnabled(false);

    m_buttonSelect = new QPushButton(tr("&Pokaz\n i\n Zamknij"));
    m_buttonSelect->setEnabled(false);

    m_listTask = new QListWidget;
    m_listTask->setSelectionMode(QAbstractItemView::MultiSelection);

    m_buttonClose = new QPushButton(tr("&Anuluj"));

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
            this, SLOT(enableFindButton()));
    connect(m_buttonSelect, SIGNAL(clicked()),
            this, SLOT(selectItemsAndDone()));
    connect(m_buttonFind, SIGNAL(clicked()), this, SLOT(findTask()));
    connect(m_buttonClose, SIGNAL(clicked()), this, SLOT(close()));
    connect(m_comboAppName, SIGNAL(currentIndexChanged(int)),
            this, SLOT(enableFindButton()));

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

void FindTaskDialog::enableFindButton()
{
    if (!m_buttonFind->isEnabled())
        m_buttonFind->setEnabled(true);
    return;
}

QList<QListWidgetItem*> FindTaskDialog::getReturnValue()
{
    return retValue;
}

void FindTaskDialog::selectItemsAndDone()
{
    retValue = m_listTask->selectedItems();
    if (retValue.isEmpty()) {
        QMessageBox::warning(this, tr("Nie znaznaczono zadania..."),
            tr("Nie zaznaczono zadania. Jesli chcesz wycofac sie z szukania"
            " kliknij <b>Anuluj</b> w oknie szukania"));
        return;
    }
    done(0);
}

void FindTaskDialog::findTask()
{
    m_listTask->clear();
    m_buttonFind->setEnabled(false);
    QString query = m_editTaskName->text();
    bool comboBoxFilterActive = m_comboAppName->currentIndex() != 0;

    bool finded = false;    // Wskazuje powodzenie lub niepowodzenie szukania
    QTreeWidgetItem* top;   // Wskazuje glowne elementy na TreeWidget
    QTreeWidgetItem* child; // Wskazuje pod-zadania/elementy na TreeWidget

    // Iteracja po glownych elementach (TopLevelItems) TreeWidgeta
    for (int t = 0; t < treeWidget->topLevelItemCount(); ++t) {
        top = treeWidget->topLevelItem(t);

        // Jesli filtr comboBoxa aktywny to trzeba dopasowac wybrany wzor
        if (comboBoxFilterActive && top->text(2).compare(
            m_comboAppName->currentText(), Qt::CaseInsensitive)) {

            continue;
        }

        if (top->text(1).contains(query, Qt::CaseInsensitive)) {
            m_listTask->addItem(top->text(1));
            finded = true;
        }

        if (top->childCount() > 0) {
            for (int c = 0; c < top->childCount(); ++c) {
                child = top->child(c);
                if (child->text(1).contains(query, Qt::CaseInsensitive)) {
                    m_listTask->addItem(child->text(1));
                    finded = true;
                }
            }
        }
    }

    if (!finded)
        m_listTask->addItem(tr("Nic nie znaleziono"));
    else
        m_buttonSelect->setEnabled(true);

    // Automatyczne zaznaczanie pierwszego zadania na liscie: zabezpiecza przed
    // errorem
    m_listTask->setCurrentRow(0);
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

