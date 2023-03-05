/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "element.h"
#include "ui_element.h"

using namespace Sekura;

Element::Element(int type, QWidget *parent) : BaseItem(type, parent), ui(new Ui::Element) {
    ui->setupUi(this);
    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &Element::cmb_indexChanged);
    connect(ui->spinBox1, &QSpinBox::valueChanged, this, &Element::sb_valueChanged);
}

Element::~Element() { delete ui; }

void Element::setCaption(const QString &str) { ui->label->setText(str); }

void Element::setValue(const QVariant &val) {
    QString type = val.toString();
    if (type.left(8) == "LineEdit") {
        int t = 0;
        QString str = type.split(";")[0].remove("LineEdit").remove("(").remove(")");
        bool ok;
        t = str.toInt(&ok);
        if (!ok)
            t = 0;
        ui->comboBox->setCurrentIndex(0);
        cmb_indexChanged(0);
        ui->comboBox2->setCurrentIndex(t);
    } else if (type.left(12) == "DateTimeEdit") {
        ui->comboBox->setCurrentIndex(1);
    } else if (type.left(8) == "ComboBox") {
        ui->comboBox->setCurrentIndex(2);
    } else if (type.left(8) == "CheckBox") {
        ui->comboBox->setCurrentIndex(3);
    } else if (type.left(7) == "SpinBox") {
        ui->comboBox->setCurrentIndex(4);
        int min, max;
        QStringList lst = type.split(";")[0].remove("SpinBox").remove("(").remove(")").split(",");
        bool ok;
        min = lst[1].toInt(&ok);
        if (!ok)
            min = 0;
        max = lst[2].toInt(&ok);
        if (!ok)
            max = 0;
        ui->spinBox1->setMinimum(0);
        ui->spinBox1->setMaximum(255);
        ui->spinBox1->setValue(min);
        ui->spinBox2->setMinimum(min);
        ui->spinBox2->setMaximum(255);
        ui->spinBox2->setValue(max);
        // sb->setMinMax(min, max);
        // ptr = sb;
    } else if (type.left(8) == "DataType") {
        ui->comboBox->setCurrentIndex(5);
    } else if (type.left(10) == "ForeignKey") {
        int t = 0;
        QString str = type.split(";")[0].remove("ForeignKey").remove("(").remove(")");
        bool ok;
        t = str.toInt(&ok);
        if (!ok)
            t = 0;
        ui->comboBox->setCurrentIndex(6);
        cmb_indexChanged(6);
        ui->comboBox2->setCurrentIndex(t);
    } else if (type == "Element") {
        ui->comboBox->setCurrentIndex(7);
    } else if (type == "Text") {
        ui->comboBox->setCurrentIndex(8);
    } else {
        ui->comboBox->setCurrentIndex(0);
        cmb_indexChanged(0);
        ui->comboBox2->setCurrentIndex(0);
    }
}

QVariant Element::value() const {
    QString str = ui->comboBox->currentText();
    int index = ui->comboBox->currentIndex();
    switch (index) {
    case 0: ///< LineEdit
        if (ui->comboBox2->currentIndex() != 0) {
            str += QString("(%1)").arg(ui->comboBox2->currentIndex());
        }
        break;
    case 1: ///< DateTime
    case 2: ///< ComboBox
    case 3: ///< CheckBox
    case 5: ///< DataType
    case 7: ///< Element
    case 8: ///< Text
        break;
    case 4:
        str += QString("(0,%1,%2)").arg(ui->spinBox1->value()).arg(ui->spinBox2->value());
        break;
    case 6: ///< ForeignKey
        str += QString("(%1)").arg(ui->comboBox2->currentIndex());
        break;
    }

    return str;
}

void Element::setViewValue(const QVariant &) {}

void Element::setReadOnly() {}

void Element::cmb_indexChanged(int a) {
    switch (a) {
    case 0: ///< LineEdit
        ui->label1->setText("Type:");
        ui->label1->setHidden(false);
        ui->spinBox1->setHidden(true);
        ui->comboBox2->setHidden(false);
        ui->comboBox2->clear();
        ui->comboBox2->addItems(QStringList() << "Empty"
                                              << "New key"
                                              << "Def pin"
                                              << "..."
                                              << "... Edit form");
        ui->label2->setHidden(true);
        ui->spinBox2->setHidden(true);
        break;
    case 1: ///< DateTime
    case 2: ///< ComboBox
    case 3: ///< CheckBox
    case 5: ///< DataType
    case 7: ///< Element
    case 8: ///< Text
        ui->label1->setHidden(true);
        ui->spinBox1->setHidden(true);
        ui->comboBox2->setHidden(true);
        ui->label2->setHidden(true);
        ui->spinBox2->setHidden(true);
        break;
    case 4:
        ui->label1->setText("Min:");
        ui->label2->setText("Max:");
        ui->label1->setHidden(false);
        ui->spinBox1->setHidden(false);
        ui->comboBox2->setHidden(true);
        ui->label2->setHidden(false);
        ui->spinBox2->setHidden(false);
        break;
    case 6: ///< ForeignKey
        ui->label1->setText("Type:");
        ui->label1->setHidden(false);
        ui->spinBox1->setHidden(false);
        ui->label2->setHidden(true);
        ui->spinBox2->setHidden(true);
        ui->comboBox2->setHidden(false);
        ui->comboBox2->clear();
        ui->comboBox2->addItems(QStringList() << "Foreign Key"
                                              << "Table"
                                              << "View column"
                                              << "Type select");
        break;
    }
}

void Element::sb_valueChanged(int a) {
    if (!ui->spinBox2->isHidden()) {
        ui->spinBox2->setMinimum(a);
    }
}
