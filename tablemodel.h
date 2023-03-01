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

    /*!
     * \brief The TableModel class - модель управления табличными данными
     */
    class TableModel : public QAbstractTableModel {
        Q_OBJECT
      public:
        explicit TableModel(ModelFilter *filter, const RestSettings *settings,
                            QObject *parent = nullptr);
        //        TableModel(const QString &model, const RestSettings *settings, const QVariantMap
        //        &filter,
        //                   QObject *parent = nullptr);
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
        const QString &formEdit() const { return m_form_edit; }

        void changeIndex(const QString &table, const QString &id);

        const QString &model() const { return m_model; }

        //        const QVector<QString> &viewData() const { return m_view_data; }
        //        void setViewData(const QVector<QString> &v) { m_view_data = v; }
        //        const QVector<QString> &headers() const { return m_headers; }
        //        void setHeaders(const QVector<QString> &v) { m_headers = v; }
        QList<int> headerIndex(const QVariantList &lst);

      signals:
        void initialized();

      public slots:
        // void setFilter(const QVariantMap &filter);
        // void removeFromFilter(const QString &key);

        void filterChanged(const QString &index, const QVariantMap &value);

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
        QMap<QString, QString> m_fk;
        QString m_model;
        QString m_stretch;
        bool m_initialized;
        bool m_viewCode;
        QString m_form_edit;

        // model filter
        ModelFilter *m_modelFilter;
        QVariantMap m_filterFromDesc;
        QVariantMap m_valueFromParent;

        RestClient *m_client;
    };

} // namespace Sekura

#endif // SEKURA_TABLEMODEL_H
