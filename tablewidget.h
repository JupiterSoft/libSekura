/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_TABLEWIDGET_H
#define SEKURA_TABLEWIDGET_H

#include "basewidget.h"
#include "restsettings.h"

#include <QWidget>

namespace Sekura {

    namespace Ui {
        class TableWidget;
    }

    class TableWidget : public BaseWidget {
        Q_OBJECT

      public:
        explicit TableWidget(const QVariantMap &data, const RestSettings *settings,
                             QWidget *parent = nullptr);
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

        void changeId(const QString &table, const QString &id) override;

      private:
        Ui::TableWidget *ui;
        TableModel *m_model;
        const RestSettings *m_settings;
        QVariantMap m_data;
    };

} // namespace Sekura
#endif // SEKURA_TABLEWIDGET_H
