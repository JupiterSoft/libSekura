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

    /*!
     * \brief The RestSettings class - настройки подключения к серверу
     */
    class RestSettings : public QObject {
        Q_OBJECT
        Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)
        Q_PROPERTY(
            QSslConfiguration *sslConfig READ sslConfig WRITE setSslConfig NOTIFY sslConfigChanged)
      public:
        explicit RestSettings(QObject *parent = nullptr);
        explicit RestSettings(const RestSettings &copy);

        bool load(const QString &name = "");
        void save(const QString &name = "");

        QString path() const;
        void setPath(const QString &newPath);

        const QByteArrayMap &headers() const { return m_headers; }
        const QByteArrayMap &data() const { return m_data; }
        const QByteArray headerValue(const QString &key) const { return m_headers[key]; }
        void removeHeaderValue(const QString &key) { m_headers.remove(key); }
        const QByteArray dataValue(const QString &key) const { return m_data[key]; }

        void setHeaderValue(const QString &key, const QByteArray &val);
        void setDataValue(const QString &key, const QByteArray &val);
        void setHeaders(const QByteArrayMap &header);
        void setData(const QByteArrayMap &data);

        QSslConfiguration *sslConfig() const;
        void setSslConfig(QSslConfiguration *newSslConfig);

      signals:
        void pathChanged();
        void headerChanged(const QString &key);

        void sslConfigChanged();

      protected:
        virtual QByteArray mCompress(const QByteArray &arr, bool direct = false) const;

      private:
        QString m_path;
        QByteArrayMap m_headers;
        QByteArrayMap m_data;
        QString m_name;
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
