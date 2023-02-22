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

        void saveForm();
        void closeForm();

      signals:
        void parentReload();

      protected:
        void reload();

      private:
        Ui::ItemWidget *ui;
        QMap<QString, BaseItem *> m_items;
        QMap<QString, bool> m_refs;
        QMap<QString, bool> m_blockOnEdit;
        QMap<QString, bool> m_blockAlways;
        QVariantList m_queries;
        RestClient *m_client;
        QVariantMap m_data;
        QVariantMap m_values;
        bool m_isNew;
    };

} // namespace Sekura
#endif // SEKURA_ITEMWIDGET_H
