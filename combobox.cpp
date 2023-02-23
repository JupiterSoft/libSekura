/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "combobox.h"
#include "ui_combobox.h"

using namespace Sekura;

ComboBox::ComboBox(int type, QWidget *parent) : BaseItem(type, parent), ui(new Ui::ComboBox) {
    ui->setupUi(this);
}

ComboBox::~ComboBox() { delete ui; }

void ComboBox::setValue(const QVariant &dt) {
    m_code = dt.toString();
    ui->comboBox->setCurrentIndex(m_id2item[dt.toString()]);
}

QVariant ComboBox::value() const { return m_item2id[ui->comboBox->currentIndex()]; }

void ComboBox::setCaption(const QString &str) { ui->label->setText(str); }

void ComboBox::setModel(const QVariantList &list) {
    int i = 0;
    bool code = false;
    foreach (QVariant v, list) {
        QVariantMap map = v.toMap();
        ui->comboBox->addItem(map["name"].toString());
        m_id2item[map["id"].toString()] = i;
        m_item2id[i] = map["id"].toString();
        if (map["id"].toString() == m_code) {
            code = true;
        }
        i++;
    }
    if (code) {
        setValue(m_code);
    }
}

void ComboBox::on_comboBox_activated(int a) { emit valueChanged(m_item2id[a]); }
