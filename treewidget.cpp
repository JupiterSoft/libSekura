/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "treewidget.h"
#include "itemwidget.h"

#include "ui_treewidget.h"
#include <QTimer>

using namespace Sekura;

TreeWidget::TreeWidget(const QVariantMap &data, const RestSettings *settings, QWidget *parent)
    : BaseWidget(parent), ui(new Ui::TreeWidget), m_settings(settings), m_data(data) {
    ui->setupUi(this);
    m_model = new TreeModel(m_data["model"].toString(), settings, this);
    ui->treeView->setModel(m_model);
    this->setWindowTitle(m_data["title"].toString());
    if (m_model->isInitialized()) {
        int m = m_model->stretchField();
        if (m != -1) {
            ui->treeView->header()->setSectionResizeMode(m, QHeaderView::Stretch);
        }
        if (m_model->buttonsContains("new")) {
            ui->pbAdd->setEnabled(true);
        } else {
            ui->pbAdd->setEnabled(false);
        }
        if (m_model->buttonsContains("edit")) {
            ui->pbEdit->setEnabled(true);
        } else {
            ui->pbEdit->setEnabled(false);
        }
        if (m_model->buttonsContains("delete")) {
            ui->pbDel->setEnabled(true);
        } else {
            ui->pbDel->setEnabled(false);
        }
    } else {
        connect(m_model, &TreeModel::initialized, this, [this]() {
            int m = m_model->stretchField();
            if (m != -1) {
                QTimer::singleShot(100, this, [this, m]() {
                    ui->treeView->header()->setSectionResizeMode(m, QHeaderView::Stretch);
                });
            }
            if (m_model->buttonsContains("new")) {
                ui->pbAdd->setEnabled(true);
            } else {
                ui->pbAdd->setEnabled(false);
            }
            if (m_model->buttonsContains("edit")) {
                ui->pbEdit->setEnabled(true);
            } else {
                ui->pbEdit->setEnabled(false);
            }
            if (m_model->buttonsContains("delete")) {
                ui->pbDel->setEnabled(true);
            } else {
                ui->pbDel->setEnabled(false);
            }
        });
    }
}

TreeWidget::~TreeWidget() { delete ui; }

void TreeWidget::on_pbAdd_clicked() {
    QVariantMap data = m_data;
    QVariantMap filter;
    data["filter"] = filter;
    // ItemWidget *item =
    // new ItemWidget(data, m_settings, this); // Item2VWidget Item2HWidget Item3HWidget
    BaseWidget *item = Interface::createWidget(m_model->formEdit(), m_settings, data, this);
    connect(item, &BaseWidget::parentReload, this, [=]() { m_model->reload(); });
    emit appendWidget(item);
}

void TreeWidget::on_pbEdit_clicked() {
    QModelIndexList selection = ui->treeView->selectionModel()->selectedIndexes();
    foreach (QModelIndex sel, selection) {
        qDebug() << m_model->code(sel);
        QString code = m_model->code(sel);
        QVariantMap data = m_data;
        QVariantMap filter;
        filter["id"] = code;
        data["filter"] = filter;
        // ItemWidget *item = new ItemWidget(data, m_settings, this);
        BaseWidget *item = Interface::createWidget(m_model->formEdit(), m_settings, data, this);
        connect(item, &BaseWidget::parentReload, this, [=]() { m_model->reload(); });
        emit appendWidget(item);
        break;
    }
}

void TreeWidget::on_pbDel_clicked() {
    QModelIndexList selection = ui->treeView->selectionModel()->selectedIndexes();
    foreach (QModelIndex sel, selection) {
        m_model->remove(sel);
    }
}
