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
    _settings = nullptr;
    manager = new QNetworkAccessManager(this);
}

RestClient::RestClient(RestSettings *settings, QObject *parent)
    : QObject(parent), _settings(settings) {
    manager = new QNetworkAccessManager(this);
}

void RestClient::request(const QString &requestString, const QString &type,
                         const QVariantMap &body) {
    QNetworkRequest request;
    QString url = (_settings ? _settings->path() : "") + requestString;
    request.setUrl(QUrl(url));
    if (_settings) {
        const QByteArrayMap &map = _settings->headers();
        for (QByteArrayMap::ConstIterator it = map.constBegin(); it != map.constEnd(); ++it) {
            request.setRawHeader(it.key().toUtf8(), it.value());
        }
        if (_settings->sslConfig() != nullptr) {
            request.setSslConfiguration(*(_settings->sslConfig()));
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
        reply = manager->sendCustomRequest(request, type.toUtf8(), buff);
        buff->setParent(reply);
    } else if (type == "POST") {
        reply = manager->post(request, bodyArray);
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
                    obj["NetworkError"] = reply->errorString();
                    emit this->error(obj);
                }
            }
            reply->close();
            reply->deleteLater();
        });
}
