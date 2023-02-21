/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "combobox.h"
#include "ui_combobox.h"

using namespace Sekura;

ComboBox::ComboBox(QWidget *parent) : QWidget(parent), ui(new Ui::ComboBox) { ui->setupUi(this); }

ComboBox::~ComboBox() { delete ui; }

void ComboBox::setModel(const QVariantList &list) {
    int i = 0;
    foreach (QVariant v, list) {
        QVariantMap map = v.toMap();
        ui->comboBox->addItem(map["name"].toString());
        m_id2item[map["id"].toString()] = i;
        m_item2id[i] = map["id"].toString();
        i++;
    }
}

void ComboBox::setCurrentId(const QString &id) { ui->comboBox->setCurrentIndex(m_id2item[id]); }

void ComboBox::setCaption(const QString &str) { ui->label->setText(str); }

QString ComboBox::currentId() const { return m_item2id[ui->comboBox->currentIndex()]; }

void ComboBox::on_comboBox_activated(int a) { emit valueChanged(m_item2id[a]); }
