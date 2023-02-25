/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "lineedit.h"
#include "sekura.h"
#include "ui_lineedit.h"

#include <QCryptographicHash>

using namespace Sekura;

/*!
 * \brief LineEdit::LineEdit - конструктор создания элемента
 * \param type - тип, может принимать значения 0, 1, 2, 3
 * \param parent - родительская форма
 */
LineEdit::LineEdit(int type, QWidget *parent) : BaseItem(type, parent), ui(new Ui::LineEdit) {
    ui->setupUi(this);
    switch (m_type) {
    case 0:
        ui->pushButton->setVisible(false);
        break;
    case 1:
        ui->pushButton->setText(tr("New key"));
        setBlock(true);
        connect(ui->pushButton, &QPushButton::clicked, this,
                [this]() { ui->lineEdit->setText(Interface ::genKey()); });
        break;
    case 2:
        ui->pushButton->setText(tr("Def pin"));
        setBlock(true);
        connect(ui->pushButton, &QPushButton::clicked, this, [this]() {
            ui->lineEdit->setText(
                QCryptographicHash::hash("1234", QCryptographicHash::Sha256).toBase64());
        });
        break;
    case 3:
        // ...
        connect(ui->pushButton, &QPushButton::clicked, this, [=]() { emit valueChanged(m_table); });
        break;
    }
}

LineEdit::~LineEdit() { delete ui; }

/*!
 * \brief LineEdit::setValue - установить значение, если режим 3, то устанавливает код
 * \param val - значение
 */
void LineEdit::setValue(const QVariant &val) {
    if (m_type == 3)
        m_code = val.toString();
    else
        ui->lineEdit->setText(val.toString());
}

/*!
 * \brief LineEdit::value - получить значение
 * \return возвращает значение элемента управления, если режим 3, то возвращает код
 */
QVariant LineEdit::value() const {
    if (m_type == 3)
        return m_code;
    return ui->lineEdit->text();
}

/*!
 * \brief LineEdit::setCaption - установка заголовка
 * \param str - значение заголовка
 */
void LineEdit::setCaption(const QString &str) { ui->label->setText(str); }

/*!
 * \brief LineEdit::setViewValue - установить видимое значение
 * \param val - значение
 */
void LineEdit::setViewValue(const QVariant &val) {
    if (m_type == 3)
        ui->lineEdit->setText(val.toString());
}

/*!
 * \brief LineEdit::selectedValues - устанавливает код и видимое значение
 * \param code - код
 * \param value - значение
 */
void LineEdit::selectedValues(const QString &code, const QString &value) {
    if (m_type == 3) {
        m_code = code;
        ui->lineEdit->setText(value);
    }
}

/*!
 * \brief LineEdit::setBlock - заблокировать элемент
 * \param t - параметр блокировки
 */
void LineEdit::setBlock(bool t) { ui->lineEdit->setReadOnly(t); }
