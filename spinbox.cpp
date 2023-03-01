/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "spinbox.h"
#include "ui_spinbox.h"

using namespace Sekura;

/*!
 * \brief SpinBox::SpinBox - конструктор элемент выбора числового значения
 * \param type - тип
 * \param parent - родительский элемент
 */
SpinBox::SpinBox(int type, QWidget *parent) : BaseItem(type, parent), ui(new Ui::SpinBox) {
    ui->setupUi(this);
}

SpinBox::~SpinBox() { delete ui; }

/*!
 * \brief SpinBox::setMinMax - установка минимального и максимального значения элементов
 * \param min - минимум
 * \param max - максимум
 */
void SpinBox::setMinMax(int min, int max) {
    ui->spinBox->setMinimum(min);
    ui->spinBox->setMaximum(max);
}

/*!
 * \brief SpinBox::setValue - установить значение элемента
 * \param val - новое значение
 */
void SpinBox::setValue(const QVariant &val) { ui->spinBox->setValue(val.toInt()); }

/*!
 * \brief SpinBox::setCaption - установить заголовок элемента
 * \param str - значение заголовка
 */
void SpinBox::setCaption(const QString &str) { ui->label->setText(str); }

/*!
 * \brief SpinBox::value - получить текущее значение элемента
 * \return значение элемента
 */
QVariant SpinBox::value() const { return ui->spinBox->value(); }

void SpinBox::setReadOnly() { ui->spinBox->setReadOnly(true); }
