/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_TABLEWIDGET_H
#define SEKURA_TABLEWIDGET_H

#include "restsettings.h"

#include <QWidget>

namespace Sekura {

    namespace Ui {
        class TableWidget;
    }

    class TableWidget : public QWidget {
        Q_OBJECT

      public:
        explicit TableWidget(const QVariantMap &data, const RestSettings *settings,
                             QWidget *parent = nullptr);
        ~TableWidget();

      public slots:
        void on_pbAdd_clicked();
        void on_pbEdit_clicked();
        void on_pnDel_clicked();

      private:
        Ui::TableWidget *ui;
        TableModel *m_model;
        QString m_dialogName;
    };

} // namespace Sekura
#endif // SEKURA_TABLEWIDGET_H
