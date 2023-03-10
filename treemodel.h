/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_TREEMODEL_H
#define SEKURA_TREEMODEL_H

#include "sekura.h"

#include <QAbstractItemModel>

namespace Sekura {

    /*!
     * \brief The TreeModel class модель управления данными в виде дерева
     */
    class TreeModel : public QAbstractItemModel {
        Q_OBJECT
      public:
        explicit TreeModel(ModelFilter *filter, QObject *parent = nullptr);
        ~TreeModel();

        QVariant data(const QModelIndex &index, int role) const override;
        QVariant headerData(int section, Qt::Orientation orientation,
                            int role = Qt::DisplayRole) const override;
        QModelIndex index(int row, int column,
                          const QModelIndex &parent = QModelIndex()) const override;
        QModelIndex parent(const QModelIndex &index) const override;
        int rowCount(const QModelIndex &parent = QModelIndex()) const override;
        int columnCount(const QModelIndex &parent = QModelIndex()) const override;

        bool isInitialized() const { return m_initialized; }
        int stretchField() const;
        bool buttonsContains(const QString &str) { return m_buttons.contains(str); }

        QString code(const QModelIndex &index);
        void remove(const QModelIndex &index);

        void makeValues(QVariantMap &map, const QStringList &header);

        void reload();

        const QString &formEdit() const { return m_form_edit; }

        void changeIndex(const QString &table, const QString &id);
        const QString &model() const { return m_model; }
        QList<int> headerIndex(const QVariantList &lst);

      signals:
        void initialized();

      public slots:
        // void setFilter(const QVariantMap &filter);
        // void removeFromFilter(const QString &key);

        void filterChanged(const QString &index, const QVariantMap &value);

      protected slots:
        void success(const QJsonObject &obj);
        void error(const QJsonObject &obj);

      private:
        class TreeItem;
        TreeItem *m_root;
        RestClient *m_client;
        QString m_model;
        bool m_initialized;
        QVariantMap m_filter;
        QVariantList m_headers;
        QVariantList m_view;
        QVariantList m_fields;
        QVariantList m_buttons;
        QString m_stretch;
        QString m_form_edit;
        QMap<QString, QString> m_fk;
        bool m_only_my;

        // model filter
        ModelFilter *m_modelFilter;
        QVariantMap m_filterFromDesc;
        QVariantMap m_valueFromParent;
    };

} // namespace Sekura

#endif // SEKURA_TREEMODEL_H
