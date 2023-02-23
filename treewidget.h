/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_TREEWIDGET_H
#define SEKURA_TREEWIDGET_H

#include "restsettings.h"
#include "treemodel.h"

#include <QWidget>

namespace Sekura {

    namespace Ui {
        class TreeWidget;
    }

    class TreeWidget : public QWidget {
        Q_OBJECT

      public:
        explicit TreeWidget(const QVariantMap &data, const RestSettings *settings,
                            QWidget *parent = nullptr);
        ~TreeWidget();

      signals:
        void appendWidget(QWidget *);

      public slots:
        void on_pbAdd_clicked();
        void on_pbEdit_clicked();
        void on_pbDel_clicked();

      private:
        Ui::TreeWidget *ui;
        const RestSettings *m_settings;
        QVariantMap m_data;
        TreeModel *m_model;
    };

} // namespace Sekura
#endif // SEKURA_TREEWIDGET_H