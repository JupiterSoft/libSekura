/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_TABLEMODEL_H
#define SEKURA_TABLEMODEL_H

#include "sekura.h"
#include <QAbstractTableModel>

namespace Sekura {

    class TableModel : public QAbstractTableModel {
        Q_OBJECT
      public:
        explicit TableModel(const QString &model, RestSettings *settings,
                            QObject *parent = nullptr);
        ~TableModel();

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;

        QString code(const QModelIndex &index) const;
        void reload();
        void remove(const QModelIndex &index);
        void setFilter(const QVariantMap &filter);

      protected slots:
        void success(const QJsonObject &);
        void error(const QJsonObject &);

      protected:
        QVector<QVariantMap> m_data;
        QVector<QString> m_view_data;
        QVector<QString> m_headers;
        QVector<QString> m_codes;
        QVariantList m_fields;
        QVariantMap m_filter;
        QString m_model;
        bool m_viewCode;

        RestClient *m_client;
    };

} // namespace Sekura

#endif // SEKURA_TABLEMODEL_H
