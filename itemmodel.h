/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_ITEMMODEL_H
#define SEKURA_ITEMMODEL_H

#include "restclient.h"

#include <QObject>

namespace Sekura {

    class ItemModel : public QObject {
        Q_OBJECT
      public:
        explicit ItemModel(const QVariantMap &map, const RestSettings *settings,
                           QObject *parent = nullptr);
        ~ItemModel();

        void save();
        void reload();
        void setItem(const QString &index, BaseItem *item);

      signals:
        void connectInterface(const QVariant &);
        void setEnabled(bool);
        void parentReload();

      protected slots:
        void success(const QJsonObject &);
        void error(const QJsonObject &);

      private:
        QMap<QString, BaseItem *> m_items;
        QMap<QString, bool> m_refs;
        QMap<QString, bool> m_blockOnEdit;
        QMap<QString, bool> m_blockAlways;
        QMap<QString, QString> m_captions;
        QVariantList m_queries;
        RestClient *m_client;
        QVariantMap m_data;
        QVariantMap m_values;
        bool m_isNew;
    };

} // namespace Sekura

#endif // SEKURA_ITEMMODEL_H
