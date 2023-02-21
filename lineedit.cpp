/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "lineedit.h"
#include "sekura.h"
#include "ui_lineedit.h"

#include <QCryptographicHash>

namespace Sekura {

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
            connect(ui->pushButton, &QPushButton::clicked, this,
                    [=]() { emit valueChanged(QVariant()); });
            break;
        }
    }

    LineEdit::~LineEdit() { delete ui; }

    void LineEdit::setValue(const QVariant &val) { ui->lineEdit->setText(val.toString()); }

    QVariant LineEdit::value() const { return ui->lineEdit->text(); }

    void LineEdit::setCaption(const QString &str) { ui->label->setText(str); }

    void LineEdit::setBlock(bool t) { ui->lineEdit->setReadOnly(t); }

} // namespace Sekura
