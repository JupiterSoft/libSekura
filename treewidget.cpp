/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "treewidget.h"

#include "modelfilter.h"
#include "ui_treewidget.h"
#include <QDialog>
#include <QTimer>

using namespace Sekura;

/*!
 * \brief TreeWidget::TreeWidget - конструктор создания формы дерева
 * \param data - данные
 * \param settings - настройки подключения
 * \param parent - родительскйи объект
 */
TreeWidget::TreeWidget(ModelFilter *filter, QWidget *parent)
    : BaseWidget(filter, parent), ui(new Ui::TreeWidget) {
    ui->setupUi(this);
    m_model = new TreeModel(filter, this);
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
                    if (m_modelFilter->contains("temp", "hidden")) {
                        QList<int> v =
                            m_model->headerIndex(m_modelFilter->value("temp", "hidden").toList());
                        foreach (int m, v)
                            if (m != -1) {
                                ui->treeView->hideColumn(m);
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

TreeWidget::~TreeWidget() { delete ui; }

/*!
 * \brief TreeWidget::on_pbAdd_clicked - добавление нового элемента
 */
void TreeWidget::on_pbAdd_clicked() { openOnEdit(); }

/*!
 * \brief TreeWidget::on_pbEdit_clicked - редактирование элемента
 */
void TreeWidget::on_pbEdit_clicked() {
    QModelIndexList selection = ui->treeView->selectionModel()->selectedIndexes();
    foreach (QModelIndex sel, selection) {
        openOnEdit(sel);
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

void TreeWidget::on_treeView_clicked(const QModelIndex &index) { m_model->code(index); }

void TreeWidget::on_treeView_doubleClicked(const QModelIndex &index) { openOnEdit(index); }

void TreeWidget::openOnEdit(const QModelIndex &index) {
    if (index.isValid()) {
        QString code = m_model->code(index);
        qDebug() << code;
        ModelFilter *mf = new ModelFilter(m_modelFilter);
        mf->setValue("temp", "model", m_model->model());
        BaseWidget *item = Interface::createWidget(mf, m_model->formEdit(), this);
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
 * \brief TreeWidget::changeId - обработка события изменения индекса в связанной таблице
 * \param table - таблица
 * \param id - новый индекс
 */
// void TreeWidget::changeId(const QString &table, const QString &id) {
//     qDebug() << m_model->model() << table << id;
//     m_model->changeIndex(table, id);
// }
