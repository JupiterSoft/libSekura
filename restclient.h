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

    /*!
     * \brief The RestClient class - основной клиент для связи с сервером
     */
    class RestClient : public QObject {
        Q_OBJECT
      public:
        explicit RestClient(QObject *parent = 0);
        RestClient(const RestSettings *settings, QObject *parent = 0);

      public slots:
        void request(const QString &requestString, const QString &type = "GET",
                     const QVariantMap &body = QVariantMap());

      signals:
        void success(const QJsonObject &);
        void error(const QJsonObject &);

      private:
        const RestSettings *m_settings;
        QNetworkAccessManager *m_manager;
    };

} // namespace Sekura

#endif // RESTCLIENT_H
