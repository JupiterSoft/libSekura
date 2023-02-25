/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "itemwidget.h"
#include "baseitem.h"
#include "bottombuttonswidget.h"
#include "bottomtablewidget.h"
#include "lineedit.h"
#include "tablewidget.h"
#include "ui_itemwidget.h"

#include <QDialog>
#include <QLayout>
#include <QMdiSubWindow>
#include <QSpacerItem>

using namespace Sekura;

ItemWidget::ItemWidget(const QVariantMap &map, const RestSettings *settings, QWidget *parent)
    : BaseWidget(parent), ui(new Ui::ItemWidget), m_settings(settings) {
    ui->setupUi(this);
    m_model = new ItemModel(map, settings, this);

    connect(m_model, &ItemModel::connectInterface, this, &ItemWidget::connectInterface);
    connect(m_model, &ItemModel::setEnabled, this, &ItemWidget::setEnabled);
    connect(m_model, &ItemModel::parentReload, this, &ItemWidget::parentReload);

    connect(m_model, &ItemModel::idChanged, this, &ItemWidget::idChanged);

    this->setWindowTitle(map["title"].toString());
}

ItemWidget::~ItemWidget() {
    delete ui;
    delete m_model;
}

void ItemWidget::changeId(const QString &table, const QString &id) {
    Q_UNUSED(table)
    Q_UNUSED(id)
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
            LineEdit *le = qobject_cast<LineEdit *>(ptr);
            if (le != nullptr) {
                connect(le, &LineEdit::valueChanged, this, [this, le](const QVariant &val) {
                    qDebug() << "open windows for " << val.toString();
                    QVariantMap map;
                    map["model"] = val;
                    map["title"] = tr("Select");
                    map["select"] = true;
                    QDialog *dialog = new QDialog(this);
                    QVBoxLayout *layout = new QVBoxLayout(dialog);
                    layout->setSpacing(0);
                    layout->setContentsMargins(5, 5, 5, 5);
                    TableWidget *widget = new TableWidget(map, m_settings, dialog);
                    connect(widget, &TableWidget::selectedValues, le, &LineEdit::selectedValues);
                    layout->addWidget(widget);
                    dialog->exec();

                    delete widget;
                    delete dialog;
                });
            }
        }
    }
    ui->baseLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));
    if (m_model->model() == "a_tables") {
        BottomTableWidget *bw = new BottomTableWidget(this);
        connect(bw, &BottomTableWidget::createTable, this, [=]() { m_model->createTable(); });
        connect(bw, &BottomTableWidget::dropTable, this, [=]() { m_model->dropTable(); });
        ui->baseLayout->addWidget(bw);
    }
    BottomButtonsWidget *bw = new BottomButtonsWidget(this);
    connect(bw, &BottomButtonsWidget::saveForm, this, &ItemWidget::saveForm);
    connect(bw, &BottomButtonsWidget::closeForm, this, &ItemWidget::closeForm);
    ui->baseLayout->addWidget(bw);
}

void ItemWidget::saveForm() { m_model->save(); }

void ItemWidget::closeForm() {
    if (m_mainForm)
        emit closeParent();
    // close();
    // QMdiSubWindow *obj = qobject_cast<QMdiSubWindow *>(parentWidget());
    // if (obj != nullptr)
    //     obj->close();
}

void ItemWidget::reload() { m_model->reload(); }
