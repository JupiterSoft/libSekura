/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_LISTMODEL_H
#define SEKURA_LISTMODEL_H

#include "modelfilter.h"
#include "restclient.h"

#include <QObject>

namespace Sekura {

    class ListModel : public QObject {
        Q_OBJECT
      public:
        explicit ListModel(ModelFilter *filter, QObject *parent = nullptr);
        ~ListModel();

        QStringList list() const;
        QVariantMap data(int a) const;
        const QString &model() const { return m_model; }

      protected slots:
        void success(const QJsonObject &);
        void error(const QJsonObject &);

      signals:
        void initialized();

      private:
        ModelFilter *m_modelFilter;
        RestClient *m_client;
        QVariantList m_data;
        QVariantList m_fields;
        QString m_model;
    };

} // namespace Sekura

#endif // SEKURA_LISTMODEL_H
