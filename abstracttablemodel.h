/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_ABSTRACTTABLEMODEL_H
#define SEKURA_ABSTRACTTABLEMODEL_H

#include "sekura.h"
#include <QAbstractTableModel>

namespace Sekura {

    class AbstractTableModel : public QAbstractTableModel {
        Q_OBJECT
      public:
        explicit AbstractTableModel(RestSettings *settings, QObject *parent = nullptr);
        virtual ~AbstractTableModel();

        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;
        QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;

        QString code(const QModelIndex &index) const;
        virtual void reload() = 0;
        virtual void remove(const QModelIndex &index) = 0;

      protected slots:
        virtual void success(const QJsonObject &) = 0;
        virtual void error(const QJsonObject &) = 0;

      protected:
        QVector<QVariantMap> m_data;
        QVector<QString> m_view_data;
        QVector<QString> m_headers;
        QVector<QString> m_codes;
        bool m_viewCode;

        RestClient *m_client;
    };

} // namespace Sekura

#endif // SEKURA_ABSTRACTTABLEMODEL_H
