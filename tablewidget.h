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

      signals:
        void appendWidget(QWidget *);
        void selectedValues(const QString &code, const QString &value);

      public slots:
        void on_pbAdd_clicked();
        void on_pbEdit_clicked();
        void on_pbDel_clicked();

        void on_pbSelect_clicked();
        void on_pbClose_clicked();

      private:
        Ui::TableWidget *ui;
        TableModel *m_model;
        const RestSettings *m_settings;
        QVariantMap m_data;
    };

} // namespace Sekura
#endif // SEKURA_TABLEWIDGET_H
