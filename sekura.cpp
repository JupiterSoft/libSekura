/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "sekura.h"

#include <QCryptographicHash>
#include <QDateTime>
#include <QUuid>

QString Sekura::Interface::genUFID() {
    return QCryptographicHash::hash(QUuid::createUuid().toString().toUtf8(),
                                    QCryptographicHash::Sha256)
        .toBase64(QByteArray::Base64UrlEncoding);
}

QString Sekura::Interface::genKey() {
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(QUuid::createUuid().toByteArray());
    hash.addData(QDateTime::currentDateTimeUtc().toString().toUtf8());
    QByteArray gen = hash.result();
    QByteArray m_key(gen.size() / 2, 0);
    for (int i = 0; i < m_key.size(); i++) {
        m_key[i] = gen[i * 2] ^ gen[i * 2 + 1];
    }
    return m_key.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);
}
