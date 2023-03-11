/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_CRYPTSETTINGS_H
#define SEKURA_CRYPTSETTINGS_H

#include "restsettings.h"

namespace Sekura {

    class CryptSettings : public Sekura::RestSettings {
        Q_OBJECT
      public:
        explicit CryptSettings(QObject *parent = nullptr);
        CryptSettings(const QByteArray &pubKey, QObject *parent = nullptr);

        void setPubKey(const QByteArray &pubKey) { m_pubKey = pubKey; }
        const QByteArray &pubKey() const { return m_pubKey; }

      protected:
        QByteArray mCompress(const QByteArray &arr, bool direct = false) const override;
        QByteArray m_pubKey;
    };

} // namespace Sekura

#endif // SEKURA_CRYPTSETTINGS_H
