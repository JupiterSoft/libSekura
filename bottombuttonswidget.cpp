/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "bottombuttonswidget.h"
#include "ui_bottombuttonswidget.h"

using namespace Sekura;

BottomButtonsWidget::BottomButtonsWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::BottomButtonsWidget) {
    ui->setupUi(this);
    connect(ui->pbSave, &QPushButton::clicked, this, &BottomButtonsWidget::saveForm);
    connect(ui->pbClose, &QPushButton::clicked, this, &BottomButtonsWidget::closeForm);
}

BottomButtonsWidget::~BottomButtonsWidget() { delete ui; }
