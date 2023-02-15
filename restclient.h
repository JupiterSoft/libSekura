/*
 * © 2022
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */

#ifndef RESTCLIENT_H
#define RESTCLIENT_H

#include "sekura.h"
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QObject>
#include <QVariant>

namespace Sekura {

    class RestClient : public QObject {
        Q_OBJECT
      public:
        explicit RestClient(QObject *parent = 0);
        RestClient(RestSettings *settings, QObject *parent = 0);

      public slots:
        void request(const QString &requestString, const QString &type = "GET",
                     const QVariantMap &body = QVariantMap());

      signals:
        void success(const QJsonObject &);
        void error(const QJsonObject &);

      private:
        RestSettings *_settings;
        QNetworkAccessManager *manager;
    };

} // namespace Sekura

#endif // RESTCLIENT_H
