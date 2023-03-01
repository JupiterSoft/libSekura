/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "treewidget.h"

#include "modelfilter.h"
#include "ui_treewidget.h"
#include <QTimer>

using namespace Sekura;

/*!
 * \brief TreeWidget::TreeWidget - конструктор создания формы дерева
 * \param data - данные
 * \param settings - настройки подключения
 * \param parent - родительскйи объект
 */
TreeWidget::TreeWidget(ModelFilter *filter, const RestSettings *settings, QWidget *parent)
    : BaseWidget(filter, parent), ui(new Ui::TreeWidget), m_settings(settings) {
    ui->setupUi(this);
    m_model = new TreeModel(filter, settings, this);
    ui->treeView->setModel(m_model);
    this->setWindowTitle(filter->value("temp", "caption").toString());
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

/*!
 * \brief TreeWidget::on_pbAdd_clicked - добавление нового элемента
 */
void TreeWidget::on_pbAdd_clicked() {
    // QVariantMap data = m_data;
    // QVariantMap filter;
    // data["filter"] = filter;
    //  ItemWidget *item =
    //  new ItemWidget(data, m_settings, this); // Item2VWidget Item2HWidget Item3HWidget
    m_modelFilter->setValue(m_model->model(), "isNew", true);
    BaseWidget *item =
        Interface::createWidget(m_modelFilter, m_model->formEdit(), m_settings, this);
    connect(item, &BaseWidget::parentReload, this, [=]() { m_model->reload(); });
    emit appendWidget(item);
}

/*!
 * \brief TreeWidget::on_pbEdit_clicked - редактирование элемента
 */
void TreeWidget::on_pbEdit_clicked() {
    QModelIndexList selection = ui->treeView->selectionModel()->selectedIndexes();
    foreach (QModelIndex sel, selection) {
        QString code = m_model->code(sel);
        qDebug() << code;
        BaseWidget *item =
            Interface::createWidget(m_modelFilter, m_model->formEdit(), m_settings, this);
        connect(item, &BaseWidget::parentReload, this, [=]() { m_model->reload(); });
        emit appendWidget(item);
        break;
    }
}

/*!
 * \brief TreeWidget::on_pbDel_clicked - удаление элемента
 */
void TreeWidget::on_pbDel_clicked() {
    QModelIndexList selection = ui->treeView->selectionModel()->selectedIndexes();
    foreach (QModelIndex sel, selection) {
        m_model->remove(sel);
    }
}

/*!
 * \brief TreeWidget::changeId - обработка события изменения индекса в связанной таблице
 * \param table - таблица
 * \param id - новый индекс
 */
void TreeWidget::changeId(const QString &table, const QString &id) {
    qDebug() << m_model->model() << table << id;
    m_model->changeIndex(table, id);
}
