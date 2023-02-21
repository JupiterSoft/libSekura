/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "spinbox.h"
#include "ui_spinbox.h"

using namespace Sekura;

SpinBox::SpinBox(QWidget *parent) : QWidget(parent), ui(new Ui::SpinBox) { ui->setupUi(this); }

SpinBox::~SpinBox() { delete ui; }

void SpinBox::setMinMax(int min, int max) {
    ui->spinBox->setMinimum(min);
    ui->spinBox->setMaximum(max);
}

void SpinBox::setValue(int val) { ui->spinBox->setValue(val); }

void SpinBox::setCaption(const QString &str) { ui->label->setText(str); }

int SpinBox::value() const { return ui->spinBox->value(); }
