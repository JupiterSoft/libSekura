/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "restsettings.h"

#include <QByteArray>
#include <QDataStream>
#include <QSettings>

using namespace Sekura;

RestSettings::RestSettings(QObject *parent) : QObject{parent} { m_sslConfig = nullptr; }

RestSettings::RestSettings(const RestSettings &copy) : QObject(copy.parent()) {
    m_path = copy.m_path;
    m_headers = copy.m_headers;
    m_data = copy.m_data;
    m_sslConfig = copy.m_sslConfig;
}

bool RestSettings::load(const QString &name) {
    if (name.isEmpty() && m_name.isEmpty())
        return false;
    if (!name.isEmpty())
        m_name = name;
    QSettings settings("JupiterSoft", "libSekura");
    if (!settings.contains(m_name))
        return false;
    QByteArray array = mCompress(settings.value(m_name).toByteArray(), true);
    QDataStream stream(array);
    stream.setVersion(QDataStream::Qt_6_0);

    stream >> m_path;
    quint32 count = 0;
    stream >> count;
    m_headers.clear();
    for (quint32 i = 0; i < count; i++) {
        QString key;
        stream >> key;
        stream >> m_headers[key];
    }
    stream >> count;
    m_data.clear();
    for (quint32 i = 0; i < count; i++) {
        QString key;
        stream >> key;
        stream >> m_data[key];
    }
    return true;
}

void RestSettings::save(const QString &name) {
    if (name.isEmpty() && m_name.isEmpty())
        return;
    if (!name.isEmpty())
        m_name = name;
    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_0);
    stream << m_path;
    stream << quint32(m_headers.count());
    for (QByteArrayMap::Iterator it = m_headers.begin(); it != m_headers.end(); ++it) {
        stream << it.key();
        stream << *it;
    }
    stream << quint32(m_data.count());
    for (QByteArrayMap::Iterator it = m_data.begin(); it != m_data.end(); ++it) {
        stream << it.key();
        stream << *it;
    }
    QSettings settings("JupiterSoft", "libSekura");
    settings.setValue(m_name, mCompress(array, false));
}

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

void RestSettings::setDataValue(const QString &key, const QByteArray &val) { m_data[key] = val; }

void RestSettings::setHeaders(const QByteArrayMap &headers) {
    m_headers = headers;
    emit headerChanged("");
}

void RestSettings::setData(const QByteArrayMap &data) { m_data = data; }

QSslConfiguration *RestSettings::sslConfig() const { return m_sslConfig; }

void RestSettings::setSslConfig(QSslConfiguration *newSslConfig) {
    if (m_sslConfig == newSslConfig)
        return;
    m_sslConfig = newSslConfig;
    emit sslConfigChanged();
}

QByteArray RestSettings::mCompress(const QByteArray &arr, bool direct) {
    if (direct)
        return qUncompress(arr);
    return qCompress(arr);
}
