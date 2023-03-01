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

    /*!
     * \brief The ItemModel class - модель для работы с элементом данных
     */
    class ItemModel : public QObject {
        Q_OBJECT
      public:
        explicit ItemModel(ModelFilter *filter, const RestSettings *settings,
                           QObject *parent = nullptr);
        ~ItemModel();

        void save();
        void reload();
        void setItem(const QString &index, BaseItem *item);

        ModelFilter *modelFilter() { return m_modelFilter; }

        void setNew();

        const QString &model() const { return m_model; }

        bool isNew() const { return m_isNew; }

        void createTable();
        void dropTable();

      signals:
        void connectInterface(const QVariant &);
        void setEnabled(bool);
        void parentReload();
        void idChanged(const QString &model, const QString &id);

      public slots:
        //        void setFilter(const QVariantMap &filter);
        //        void removeFromFilter(const QString &key);

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

        // QVariantMap m_data;
        // QVariantMap m_filter;
        QVariantList m_fields;
        QVariantMap m_values;
        QString m_model;

        // model filter
        ModelFilter *m_modelFilter;
        QVariantMap m_filterFromDesc;
        QVariantMap m_valueFromParent;

        bool m_isNew;
    };

} // namespace Sekura

#endif // SEKURA_ITEMMODEL_H
