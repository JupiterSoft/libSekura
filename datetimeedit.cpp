/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "datetimeedit.h"
#include "ui_datetimeedit.h"

using namespace Sekura;

DateTimeEdit::DateTimeEdit(QWidget *parent) : QWidget(parent), ui(new Ui::DateTimeEdit) {
    ui->setupUi(this);
}

DateTimeEdit::~DateTimeEdit() { delete ui; }

void DateTimeEdit::setValue(const QDateTime &dt) { ui->dateTimeEdit->setDateTime(dt); }

QDateTime DateTimeEdit::value() const { return ui->dateTimeEdit->dateTime(); }

void DateTimeEdit::setCaption(const QString &str) { ui->label->setText(str); }
