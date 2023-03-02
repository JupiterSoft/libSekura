/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "wizarditemwidget.h"
#include "ui_wizarditemwidget.h"

using namespace Sekura;

WizardItemWidget::WizardItemWidget(const QString &form, QWidget *parent)
    : QDialog(parent), ui(new Ui::WizardItemWidget), m_form(form) {
    ui->setupUi(this);
    connect(ui->pbClose, &QPushButton::clicked, this, &WizardItemWidget::reject);
    connect(ui->pbSave, &QPushButton::clicked, this, &WizardItemWidget::pbSave_clicked);
    connect(ui->pbAdd, &QPushButton::clicked, this, &WizardItemWidget::pbAdd_clicked);
    connect(ui->pbAddChild, &QPushButton::clicked, this, &WizardItemWidget::pbAddChild_clicked);
    connect(ui->pbMove, &QPushButton::clicked, this, &WizardItemWidget::pbMove_clicked);
    connect(ui->pbDel, &QPushButton::clicked, this, &WizardItemWidget::pbDel_clicked);
    connect(ui->cmbItem, &QComboBox::currentIndexChanged, this,
            &WizardItemWidget::cmbItem_currentChanged);
    connect(ui->cmbModel, &QComboBox::currentIndexChanged, this,
            &WizardItemWidget::cmbModel_currentChanged);

    connect(ui->tree, &QTreeWidget::currentItemChanged, this,
            &WizardItemWidget::tree_currentItemChanged);
    connect(ui->tw, &QTableWidget::itemChanged, this, &WizardItemWidget::tw_itemChanged);
}

WizardItemWidget::~WizardItemWidget() { delete ui; }

void WizardItemWidget::pbSave_clicked() {
    /// TODO make form
    QDialog::accept();
}

void WizardItemWidget::pbAdd_clicked() {}

void WizardItemWidget::pbAddChild_clicked() {}

void WizardItemWidget::pbMove_clicked() {}

void WizardItemWidget::pbDel_clicked() {}

void WizardItemWidget::cmbItem_currentChanged(int a) {
    switch (a) {
    case 0: ///< Item
    case 1: ///< Table
    case 2: ///< Tree
        ui->sw->setCurrentIndex(0);
        break;
    case 3: ///< Box
    case 4: ///< Splitter
    case 5: ///< Scroll
    case 6: ///< Tab
    case 7: ///< Tool
        ui->sw->setCurrentIndex(1);
        break;
    }
}

void WizardItemWidget::cmbModel_currentChanged(int a) {}

void WizardItemWidget::tree_currentItemChanged(QTreeWidgetItem *current,
                                               QTreeWidgetItem *previous) {}

void WizardItemWidget::tw_itemChanged(QTableWidgetItem *item) {}
