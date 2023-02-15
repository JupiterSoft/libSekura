/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "sekura.h"

#include <QCryptographicHash>
#include <QUuid>

QString Sekura::Interface::genUFID() {
    return QCryptographicHash::hash(QUuid::createUuid().toString().toUtf8(),
                                    QCryptographicHash::Sha256)
        .toBase64(QByteArray::Base64UrlEncoding);
}
