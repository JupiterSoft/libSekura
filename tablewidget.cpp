/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "tablewidget.h"
#include "itemwidget.h"
#include "tablemodel.h"
#include "ui_tablewidget.h"
#include <QDialog>
#include <QTimer>

using namespace Sekura;

TableWidget::TableWidget(const QVariantMap &data, const RestSettings *settings, QWidget *parent)
    : QWidget(parent), ui(new Ui::TableWidget), m_settings(settings), m_data(data) {
    ui->setupUi(this);
    if (m_data.contains("select")) {
        ui->gbTop->setVisible(false);
    } else {
        ui->gbBottom->setVisible(false);
    }
    if (data.contains("filter")) {
        m_model =
            new TableModel(m_data["model"].toString(), settings, m_data["filter"].toMap(), this);
    } else {
        m_model = new TableModel(m_data["model"].toString(), settings, this);
    }
    ui->tableView->setModel(m_model);
    this->setWindowTitle(m_data["title"].toString());
    if (m_model->isInitialized()) {
        int m = m_model->stretchField();
        if (m != -1) {
            ui->tableView->horizontalHeader()->setSectionResizeMode(m, QHeaderView::Stretch);
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
        connect(m_model, &TableModel::initialized, this, [this]() {
            int m = m_model->stretchField();
            if (m != -1) {
                QTimer::singleShot(100, this, [this, m]() {
                    ui->tableView->horizontalHeader()->setSectionResizeMode(m,
                                                                            QHeaderView::Stretch);
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

TableWidget::~TableWidget() { delete ui; }

void TableWidget::on_pbAdd_clicked() {
    /// TODO добавить вставку
    QVariantMap data = m_data;
    QVariantMap filter;
    data["filter"] = filter;
    ItemWidget *item =
        new ItemWidget(data, m_settings, this); // Item2VWidget Item2HWidget Item3HWidget
    connect(item, &ItemWidget::parentReload, this, [=]() { m_model->reload(); });
    emit appendWidget(item);
}

void TableWidget::on_pbEdit_clicked() {
    QModelIndexList selection = ui->tableView->selectionModel()->selectedIndexes();
    foreach (QModelIndex sel, selection) {
        qDebug() << m_model->code(sel);
        QString code = m_model->code(sel);
        QVariantMap data = m_data;
        QVariantMap filter;
        filter["id"] = code;
        data["filter"] = filter;
        ItemWidget *item = new ItemWidget(data, m_settings, this);
        connect(item, &ItemWidget::parentReload, this, [=]() { m_model->reload(); });
        emit appendWidget(item);
        break;
    }
    /// TODO добавить редактирование
}

void TableWidget::on_pbDel_clicked() {
    /// TODO добавить удаление
    QModelIndexList selection = ui->tableView->selectionModel()->selectedIndexes();
    foreach (QModelIndex sel, selection) {
        m_model->remove(sel);
    }
}

void TableWidget::on_pbSelect_clicked() {
    QDialog *dialog = qobject_cast<QDialog *>(parentWidget());
    if (dialog != nullptr) {
        QModelIndexList selection = ui->tableView->selectionModel()->selectedIndexes();
        foreach (QModelIndex sel, selection) {
            qDebug() << m_model->code(sel);
            QString code = m_model->code(sel);
            QString value = m_model->value(sel);
            emit selectedValues(code, value);
            dialog->accept();
            break;
        }
    }
}

void TableWidget::on_pbClose_clicked() {
    QDialog *dialog = qobject_cast<QDialog *>(parentWidget());
    if (dialog != nullptr)
        dialog->reject();
}
