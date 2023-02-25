/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "tablewidget.h"
#include "tablemodel.h"
#include "ui_tablewidget.h"
#include <QDialog>
#include <QTimer>

using namespace Sekura;

/*!
 * \brief TableWidget::TableWidget - конструктор
 * \param data - данные
 * \param settings - настройки подключения
 * \param parent - родительский объект
 */
TableWidget::TableWidget(const QVariantMap &data, const RestSettings *settings, QWidget *parent)
    : BaseWidget(parent), ui(new Ui::TableWidget), m_settings(settings), m_data(data) {
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

/*!
 * \brief TableWidget::on_pbAdd_clicked - нажатие на кнопку добавить
 */
void TableWidget::on_pbAdd_clicked() {
    /// TODO добавить вставку
    QVariantMap data = m_data;
    QVariantMap filter;
    data["filter"] = filter;
    // ItemWidget *item =
    //     new ItemWidget(data, m_settings, this); // Item2VWidget Item2HWidget Item3HWidget

    BaseWidget *item = Interface::createWidget(m_model->formEdit(), m_settings, data, this);

    connect(item, &BaseWidget::parentReload, this, [=]() { m_model->reload(); });
    emit appendWidget(item);
}

/*!
 * \brief TableWidget::on_pbEdit_clicked - нажатие на кнопку редактировать
 */
void TableWidget::on_pbEdit_clicked() {
    QModelIndexList selection = ui->tableView->selectionModel()->selectedIndexes();
    foreach (QModelIndex sel, selection) {
        qDebug() << m_model->code(sel);
        QString code = m_model->code(sel);
        QVariantMap data = m_data;
        QVariantMap filter;
        filter["id"] = code;
        data["filter"] = filter;
        BaseWidget *item = Interface::createWidget(m_model->formEdit(), m_settings, data, this);
        item->setMainForm(true);
        // ItemWidget *item = new ItemWidget(data, m_settings, this);
        connect(item, &BaseWidget::parentReload, this, [=]() { m_model->reload(); });
        emit appendWidget(item);
        break;
    }
    /// TODO добавить редактирование
}

/*!
 * \brief TableWidget::on_pbDel_clicked - удалить запись
 */
void TableWidget::on_pbDel_clicked() {
    /// TODO добавить удаление
    QModelIndexList selection = ui->tableView->selectionModel()->selectedIndexes();
    foreach (QModelIndex sel, selection) {
        m_model->remove(sel);
    }
}

/*!
 * \brief TableWidget::on_pbSelect_clicked - выбор элемента
 */
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

/*!
 * \brief TableWidget::on_pbClose_clicked - закрыть форму
 */
void TableWidget::on_pbClose_clicked() {
    QDialog *dialog = qobject_cast<QDialog *>(parentWidget());
    if (dialog != nullptr)
        dialog->reject();
}

/*!
 * \brief TableWidget::on_tableView_clicked - нажатие мышкой, сообщить заинтересованным об измении
 * индекса
 * \param index - индекс
 */
void TableWidget::on_tableView_clicked(const QModelIndex &index) {
    QString code = m_model->code(index);
    if (!code.isEmpty()) {
        emit idChanged(m_model->model(), code);
    }
}

/*!
 * \brief TableWidget::on_tableView_doubleClicked - двойное нажатие в режиме выбора элемента
 * \param index выбранный индекс
 */
void TableWidget::on_tableView_doubleClicked(const QModelIndex &index) { on_pbSelect_clicked(); }

/*!
 * \brief TableWidget::changeId - обработка сообщения об изменении текущего индекса в таблице
 * \param table - таблица
 * \param id - индекс
 */
void TableWidget::changeId(const QString &table, const QString &id) {
    qDebug() << m_model->model() << table << id;
    m_model->changeIndex(table, id);
}
