/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "restsettings.h"

using namespace Sekura;

RestSettings::RestSettings(QObject *parent) : QObject{parent} { m_sslConfig = nullptr; }

QString RestSettings::path() const { return m_path; }

void RestSettings::setPath(const QString &newPath) {
    if (m_path == newPath)
        return;
    m_path = newPath;
    emit pathChanged();
}

void RestSettings::setHeaderValue(const QString &key, const QByteArray &val) {
    bool changed = false;
    if (!m_headers.contains(key) && (m_headers[key] != val))
        changed = true;
    if (changed) {
        m_headers[key] = val;
        emit headerChanged(key);
    }
}

QSslConfiguration *RestSettings::sslConfig() const { return m_sslConfig; }

void RestSettings::setSslConfig(QSslConfiguration *newSslConfig) {
    if (m_sslConfig == newSslConfig)
        return;
    m_sslConfig = newSslConfig;
    emit sslConfigChanged();
}
