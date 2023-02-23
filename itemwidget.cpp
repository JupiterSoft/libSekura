/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "itemwidget.h"
#include "baseitem.h"
#include "bottombuttonswidget.h"
#include "ui_itemwidget.h"

#include <QMdiSubWindow>
#include <QSpacerItem>

using namespace Sekura;

ItemWidget::ItemWidget(const QVariantMap &map, const RestSettings *settings, QWidget *parent)
    : QWidget(parent), ui(new Ui::ItemWidget) {
    ui->setupUi(this);
    m_model = new ItemModel(map, settings, this);

    connect(m_model, &ItemModel::connectInterface, this, &ItemWidget::connectInterface);
    connect(m_model, &ItemModel::setEnabled, this, &ItemWidget::setEnabled);
    connect(m_model, &ItemModel::parentReload, this, &ItemWidget::parentReload);

    this->setWindowTitle(map["title"].toString());
}

ItemWidget::~ItemWidget() {
    delete ui;
    delete m_model;
}

void ItemWidget::connectInterface(const QVariant &val) {
    QVariantList fields = val.toList();
    QVariantList mf;
    foreach (QVariant v, fields) {
        QVariantMap m = v.toMap();
        QString id = m["name"].toString();
        mf << id;
        BaseItem *ptr = Sekura::Interface::createItem(m, this);
        if (ptr != nullptr) {
            ui->baseLayout->addWidget(ptr);
            m_model->setItem(id, ptr);
        }
    }
    ui->baseLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    BottomButtonsWidget *bw = new BottomButtonsWidget(this);
    connect(bw, &BottomButtonsWidget::saveForm, this, &ItemWidget::saveForm);
    connect(bw, &BottomButtonsWidget::closeForm, this, &ItemWidget::closeForm);
    ui->baseLayout->addWidget(bw);
}

void ItemWidget::saveForm() { m_model->save(); }

void ItemWidget::closeForm() {
    close();
    QMdiSubWindow *obj = qobject_cast<QMdiSubWindow *>(parentWidget());
    if (obj != nullptr)
        obj->close();
}

void ItemWidget::reload() { m_model->reload(); }
