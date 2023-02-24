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
        explicit TableModel(const QString &model, const RestSettings *settings,
                            QObject *parent = nullptr);
        TableModel(const QString &model, const RestSettings *settings, const QVariantMap &filter,
                   QObject *parent = nullptr);
        ~TableModel();

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;

        bool isInitialized() const { return m_initialized; }
        int stretchField() const;

        QString code(const QModelIndex &index) const;
        QString value(const QModelIndex &index) const;

        void reload();
        void remove(const QModelIndex &index);

        bool buttonsContains(const QString &str) { return m_buttons.contains(str); }

      signals:
        void initialized();

      public slots:
        void setFilter(const QVariantMap &filter);
        void removeFromFilter(const QString &key);

      protected slots:
        void success(const QJsonObject &);
        void error(const QJsonObject &);

      protected:
        QVector<QVariantMap> m_data;
        QVector<QString> m_view_data;
        QVector<QString> m_headers;
        QVector<QString> m_codes;
        QVariantList m_buttons;
        QVariantList m_fields;
        QVariantMap m_filter;
        QString m_model;
        QString m_stretch;
        bool m_initialized;
        bool m_viewCode;

        RestClient *m_client;
    };

} // namespace Sekura

#endif // SEKURA_TABLEMODEL_H
