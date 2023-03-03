/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "datatype.h"
#include "ui_datatype.h"

using namespace Sekura;

DataType::DataType(int type, QWidget *parent) : BaseItem(type, parent), ui(new Ui::DataType) {
    ui->setupUi(this);
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &DataType::cmb_indexChanged);
    connect(ui->checkBox, &QCheckBox::stateChanged, this, &DataType::cb_check);
    ui->checkBox->setHidden(true);
    ui->spinBox->setHidden(true);
    QWidget::setObjectName("DataType_local");
}

DataType::~DataType() { delete ui; }

void DataType::setCaption(const QString &str) { ui->label->setText(str); }

void DataType::setValue(const QVariant &val) {
    QString str = val.toString();
    if (str == "UUID") {
        /// index
        ui->comboBox->setCurrentIndex(0);
    } else if (str == "TIMESTAMP WITH TIME ZONE") {
        /// DateTime
        ui->comboBox->setCurrentIndex(1);
    } else if (str == "BOOLEAN") {
        /// Bool
        ui->comboBox->setCurrentIndex(2);
    } else if (str == "INTEGER") {
        /// int
        ui->comboBox->setCurrentIndex(3);
    } else if (str == "FLOAT") {
        /// float
        ui->comboBox->setCurrentIndex(4);
    } else if (str.left(7) == "VARCHAR") {
        /// varchar
        ui->comboBox->setCurrentIndex(5);
        QString m = str.remove("VARCHAR(").remove(")");
        if (m == "65536") {
            ui->spinBox->setHidden(true);
            ui->checkBox->setChecked(true);
        } else {
            ui->checkBox->setChecked(false);
            ui->spinBox->setValue(m.toInt());
        }
    }
}

QVariant DataType::value() const {
    QString ret;
    switch (ui->comboBox->currentIndex()) {
    case 0:
        ret = "UUID";
        break;
    case 1:
        ret = "TIMESTAMP WITH TIME ZONE";
        break;
    case 2:
        ret = "BOOLEAN";
        break;
    case 3:
        ret = "INTEGER";
        break;
    case 4:
        ret = "FLOAT";
        break;
    case 5:
        ret = QString("VARCHAR(%1)").arg(ui->checkBox->isChecked() ? 65536 : ui->spinBox->value());
        break;
    }

    return ret;
}

void DataType::setViewValue(const QVariant &) {}

void DataType::setReadOnly() {}

void DataType::cmb_indexChanged(int a) {
    if (a == 5) {
        ui->checkBox->setHidden(false);
        ui->spinBox->setHidden(false);
        ui->checkBox->setChecked(false);
    } else {
        ui->checkBox->setHidden(true);
        ui->spinBox->setHidden(true);
    }
    emit valueChanged(a);
}

void DataType::cb_check(int a) {
    if (ui->checkBox->isChecked()) {
        ui->spinBox->setHidden(true);
    } else {
        ui->spinBox->setHidden(false);
    }
}
