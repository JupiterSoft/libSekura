/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "bottomtablewidget.h"
#include "ui_bottomtablewidget.h"

using namespace Sekura;

BottomTableWidget::BottomTableWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::BottomTableWidget) {
    ui->setupUi(this);
    connect(ui->pbCreate, &QPushButton::clicked, this, &BottomTableWidget::createTable);
    connect(ui->pbDrop, &QPushButton::clicked, this, &BottomTableWidget::dropTable);
}

BottomTableWidget::~BottomTableWidget() { delete ui; }
