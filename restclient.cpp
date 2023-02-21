/*
 * © 2022
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "restclient.h"

#include "restsettings.h"
#include <QBuffer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkReply>
#include <QNetworkRequest>

using namespace Sekura;

RestClient::RestClient(QObject *parent) : QObject(parent) {
    m_settings = nullptr;
    m_manager = new QNetworkAccessManager(this);
}

RestClient::RestClient(const RestSettings *settings, QObject *parent)
    : QObject(parent), m_settings(settings) {
    m_manager = new QNetworkAccessManager(this);
}

void RestClient::request(const QString &requestString, const QString &type,
                         const QVariantMap &body) {
    QNetworkRequest request;
    QString url = (m_settings ? m_settings->path() : "") + requestString;
    request.setUrl(QUrl(url));
    if (m_settings) {
        const QByteArrayMap &map = m_settings->headers();
        for (QByteArrayMap::ConstIterator it = map.constBegin(); it != map.constEnd(); ++it) {
            request.setRawHeader(it.key().toUtf8(), it.value());
        }
        if (m_settings->sslConfig() != nullptr) {
            request.setSslConfiguration(*(m_settings->sslConfig()));
        }
    }
    QNetworkReply *reply = nullptr;
    QByteArray bodyArray = "{\"empty\":0}";
    if (!body.isEmpty()) {
        QJsonDocument postDataDoc = QJsonDocument::fromVariant(body);
        bodyArray = postDataDoc.toJson();
    }
    // manager->
    if ((type == "GET") || (type == "PATCH") || (type == "DELETE")) {
        request.setRawHeader("HTTP", type.toUtf8());
        QBuffer *buff = new QBuffer;
        buff->setData(bodyArray);
        buff->open(QIODevice::ReadOnly);
        reply = m_manager->sendCustomRequest(request, type.toUtf8(), buff);
        buff->setParent(reply);
    } else if (type == "POST") {
        reply = m_manager->post(request, bodyArray);
    } else {
        /// TODO make error
        reply = nullptr;
    }

    if (reply != nullptr)
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            QJsonParseError error;
            QByteArray replyData = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(replyData, &error);
            if (error.error != QJsonParseError::NoError) {
                qDebug() << "Error parse json: " << error.errorString();
            } else {
                QJsonObject obj;
                if (doc.isObject())
                    obj = doc.object();
                else if (doc.isArray())
                    obj["arr"] = doc.array();
                QNetworkReply::NetworkError replyError = reply->error();
                int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                if ((replyError == QNetworkReply::NoError) && ((code >= 200) && (code < 300))) {
                    emit this->success(obj);
                } else {
                    /// TODO вставить удаления токена авторизации по коду ошибки ответа!!
                    obj["NetworkError"] = reply->errorString();
                    emit this->error(obj);
                }
            }
            reply->close();
            reply->deleteLater();
        });
}
