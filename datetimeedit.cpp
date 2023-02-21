/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "datetimeedit.h"
#include "ui_datetimeedit.h"

using namespace Sekura;

DateTimeEdit::DateTimeEdit(int type, QWidget *parent)
    : BaseItem(type, parent), ui(new Ui::DateTimeEdit) {
    ui->setupUi(this);
}

DateTimeEdit::~DateTimeEdit() { delete ui; }

void DateTimeEdit::setValue(const QVariant &dt) { ui->dateTimeEdit->setDateTime(dt.toDateTime()); }

QVariant DateTimeEdit::value() const { return ui->dateTimeEdit->dateTime(); }

void DateTimeEdit::setCaption(const QString &str) { ui->label->setText(str); }
