/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "tablewidget.h"
#include "modelfilter.h"
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
TableWidget::TableWidget(ModelFilter *filter, QWidget *parent)
    : BaseWidget(filter, parent), ui(new Ui::TableWidget) {
    ui->setupUi(this);
    m_mode = 0; ///< Просмотр в таблице
    if (m_modelFilter->contains("temp", "select")) {
        // ui->gbTop->setVisible(false);
        m_mode = 1; ///< выбор в таблице
    } else {
        ui->gbBottom->setVisible(false);
    }
    m_model = new TableModel(m_modelFilter, this);
    ui->tableView->setModel(m_model);
    this->setWindowTitle(m_modelFilter->value("temp", "caption").toString());
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
                    if (m_modelFilter->contains("temp", "hidden")) {
                        QList<int> v =
                            m_model->headerIndex(m_modelFilter->value("temp", "hidden").toList());
                        foreach (int m, v)
                            if (m != -1) {
                                ui->tableView->setColumnHidden(m, true);
                            }
                    }
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
void TableWidget::on_pbAdd_clicked() { openToEdit(); }

/*!
 * \brief TableWidget::on_pbEdit_clicked - нажатие на кнопку редактировать
 */
void TableWidget::on_pbEdit_clicked() {
    QModelIndexList selection = ui->tableView->selectionModel()->selectedIndexes();
    foreach (QModelIndex sel, selection) {
        openToEdit(sel);
        break;
    }
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
            QString code = m_model->code(sel);
            qDebug() << code;
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
void TableWidget::on_tableView_clicked(const QModelIndex &index) { m_model->code(index); }

/*!
 * \brief TableWidget::on_tableView_doubleClicked - двойное нажатие в режиме выбора элемента
 * \param index выбранный индекс
 */
void TableWidget::on_tableView_doubleClicked(const QModelIndex &index) {
    if (m_mode == 1)
        on_pbSelect_clicked();
    else if (m_mode == 0) {
        openToEdit(index);
    }
}

void TableWidget::openToEdit(const QModelIndex &index) {
    if (index.isValid()) {
        QString code = m_model->code(index);
        qDebug() << code;
        ModelFilter *mf = new ModelFilter(m_modelFilter);
        mf->setValue("temp", "model", m_model->model());
        BaseWidget *item = Interface::createWidget(mf, m_model->formEdit(), this);
        item->setMainForm(true);
        connect(item, &BaseWidget::parentReload, this, [=]() { m_model->reload(); });
        emit appendWidget(item);
    } else {
        ModelFilter *mf = new ModelFilter(m_modelFilter);
        mf->setValue("temp", "model", m_model->model());
        mf->setValue(m_model->model(), "isNew", true);
        BaseWidget *item = Interface::createWidget(mf, m_model->formEdit(), this);
        connect(item, &BaseWidget::parentReload, this, [=]() { m_model->reload(); });
        QDialog *dialog = qobject_cast<QDialog *>(parentWidget());
        if (dialog == nullptr)
            emit appendWidget(item);
        else {
            /// TODO Создать диалог
            QDialog *dialog = new QDialog(this);
            item->setParent(dialog);
            QVBoxLayout *layout = new QVBoxLayout(dialog);
            layout->setSpacing(0);
            layout->setContentsMargins(5, 5, 5, 5);
            layout->addWidget(item);
            dialog->exec();

            delete item;
            delete dialog;
        }
    }
}

/*!
 * \brief TableWidget::changeId - обработка сообщения об изменении текущего индекса в таблице
 * \param table - таблица
 * \param id - индекс
 */
// void TableWidget::changeId(const QString &table, const QString &id) {
//     qDebug() << m_model->model() << table << id;
//     m_model->changeIndex(table, id);
// }
