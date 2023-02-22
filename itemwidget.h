/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_ITEMWIDGET_H
#define SEKURA_ITEMWIDGET_H

#include "sekura.h"
#include <QWidget>

namespace Sekura {

    namespace Ui {
        class ItemWidget;
    }

    class ItemWidget : public QWidget {
        Q_OBJECT

      public:
        explicit ItemWidget(const QVariantMap &map, const RestSettings *settings,
                            QWidget *parent = nullptr);
        ~ItemWidget();

      protected slots:
        void success(const QJsonObject &);
        void error(const QJsonObject &);

      protected:
        void reload();

      private:
        Ui::ItemWidget *ui;
        QMap<QString, BaseItem *> m_items;
        RestClient *m_client;
        QVariantMap m_data;
    };

} // namespace Sekura
#endif // SEKURA_ITEMWIDGET_H
