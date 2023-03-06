/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_TABLEWIDGET_H
#define SEKURA_TABLEWIDGET_H

#include "basewidget.h"
#include "tablemodel.h"

#include <QModelIndex>
#include <QWidget>

namespace Sekura {

    namespace Ui {
        class TableWidget;
    }

    /*!
     * \brief The TableWidget class - форма отображения табличных данных
     */
    class TableWidget : public BaseWidget {
        Q_OBJECT

      public:
        explicit TableWidget(ModelFilter *filter, QWidget *parent = nullptr);
        ~TableWidget();

      signals:
        void selectedValues(const QString &code, const QString &value);

      public slots:
        void on_pbAdd_clicked();
        void on_pbEdit_clicked();
        void on_pbDel_clicked();

        void on_pbSelect_clicked();
        void on_pbClose_clicked();

        void on_tableView_clicked(const QModelIndex &index);

        void on_tableView_doubleClicked(const QModelIndex &index);

      protected:
        void openToEdit(const QModelIndex &index = QModelIndex());

      private:
        Ui::TableWidget *ui;
        TableModel *m_model;
        // QVariantMap m_data;
        int m_mode;
    };

} // namespace Sekura
#endif // SEKURA_TABLEWIDGET_H
