/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "cryptsettings.h"
#include "qaesencryption.h"

namespace Sekura {

    QByteArray key_to_iv(const QByteArray &arr) {
        QByteArray ret(16, 0);
        for (int i = 0; i < 16; i++)
            ret[i] = arr[i * 2] ^ arr[i * 2 + 1];
        return ret;
    }

    CryptSettings::CryptSettings(QObject *parent) : Sekura::RestSettings{parent} {}

    CryptSettings::CryptSettings(const QByteArray &pubKey, QObject *parent)
        : Sekura::RestSettings{parent}, m_pubKey{pubKey} {}

    QByteArray CryptSettings::mCompress(const QByteArray &arr, bool direct) const {
        if (direct) {
            QByteArray data = qUncompress(arr);
            QByteArray key = QCryptographicHash::hash(m_pubKey, QCryptographicHash::Sha256);
            QByteArray iv = key_to_iv(key);
            return QAESEncryption::DefDecrypt(data, key, iv);
        }
        QByteArray key = QCryptographicHash::hash(m_pubKey, QCryptographicHash::Sha256);
        QByteArray iv = key_to_iv(key);
        QByteArray data = QAESEncryption::DefCrypt(arr, key, iv);
        return qCompress(data);
    }

} // namespace Sekura
