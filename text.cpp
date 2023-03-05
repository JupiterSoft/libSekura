/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "text.h"
#include "ui_text.h"

using namespace Sekura;

Text::Text(int type, QWidget *parent) : BaseItem(type, parent), ui(new Ui::Text) {
    ui->setupUi(this);
    m_document = ui->textEdit->document();
    connect(ui->pbBold, &QPushButton::clicked, this, &Text::bold);
}

Text::~Text() { delete ui; }

void Text::setValue(const QVariant &dt) { ui->textEdit->setHtml(dt.toString()); }

QVariant Text::value() const { return ui->textEdit->toHtml(); }

void Text::setCaption(const QString &str) { this->setWindowTitle(str); }

void Text::setViewValue(const QVariant &) {}

void Text::setReadOnly() { ui->groupBox->setHidden(true); }

void Text::bold() {
    QTextCursor cursor = ui->textEdit->textCursor();
    QTextCharFormat format = cursor.charFormat();
    QFont font = format.font();
    font.setBold(!font.bold());
    format.setFont(font);
    cursor.mergeCharFormat(format);
}
