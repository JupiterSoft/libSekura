/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef RESTSETTINGS_H
#define RESTSETTINGS_H

#include "sekura.h"
#include <QMap>
#include <QObject>
#include <QSslConfiguration>

namespace Sekura {

    class RestSettings : public QObject {
        Q_OBJECT
        Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
        Q_PROPERTY(
            QSslConfiguration *sslConfig READ sslConfig WRITE setSslConfig NOTIFY sslConfigChanged)
      public:
        explicit RestSettings(QObject *parent = nullptr);

        QString path() const;
        void setPath(const QString &newPath);

        const QByteArrayMap &headers() const { return m_headers; }
        const QByteArray headerValue(const QString &key) const { return m_headers[key]; }

        void setHeaderValue(const QString &key, const QByteArray &val);

        QSslConfiguration *sslConfig() const;
        void setSslConfig(QSslConfiguration *newSslConfig);

      signals:
        void pathChanged();
        void headerChanged(const QString &key);

        void sslConfigChanged();

      private:
        QString m_path;
        QByteArrayMap m_headers;
        QSslConfiguration *m_sslConfig = nullptr;
    };

    /*
     * Необходимо установить в заголовок запроса
     * Content-Type       : application/json
     * charset            : utf-8
     * Authorization      : 'TOKEN'
     * sekura-application : 'код приложения'
     */

} // namespace Sekura

#endif // RESTSETTINGS_H
