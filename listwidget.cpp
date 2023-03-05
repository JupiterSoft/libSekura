/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "listwidget.h"
#include "ui_listwidget.h"

using namespace Sekura;

ListWidget::ListWidget(ModelFilter *filter, QWidget *parent)
    : BaseWidget(filter, parent), ui(new Ui::ListWidget) {
    ui->setupUi(this);
    m_model = new ListModel(m_modelFilter, this);
    connect(m_model, &ListModel::initialized, this, &ListWidget::model_initialized);
    ui->label->setText(m_modelFilter->value("temp", "caption").toString());
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &ListWidget::cmb_currentChanged);
}

ListWidget::~ListWidget() {
    delete ui;
    delete m_model;
}

void ListWidget::cmb_currentChanged(int a) {
    m_modelFilter->setValue(m_model->model(), m_model->data(a));
}

void ListWidget::model_initialized() {
    ui->comboBox->addItems(m_model->list());
    cmb_currentChanged(0);
}
