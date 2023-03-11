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

/*!
 * \brief RestSettings::RestSettings - базовый конструктор
 * \param parent - родительский объект
 */
RestSettings::RestSettings(QObject *parent) : QObject{parent} { m_sslConfig = nullptr; }

/*!
 * \brief RestSettings::RestSettings - копирующий конструктор
 * \param copy - объект копирования
 */
RestSettings::RestSettings(const RestSettings &copy) : QObject(copy.parent()) {
    m_path = copy.m_path;
    m_headers = copy.m_headers;
    m_data = copy.m_data;
    m_sslConfig = copy.m_sslConfig;
}

/*!
 * \brief RestSettings::load - загрузить из настроек по имени
 * \param name - имя раздела
 * \return возвращает истину если загрузка успешная
 */
bool RestSettings::load(const QString &name) {
    if (name.isEmpty() && m_name.isEmpty())
        return false;
    if (!name.isEmpty())
        m_name = name;
    QSettings settings("JupiterSoft", "libSekura");
    if (m_name == "Default")
        m_name = settings.value("Default").toString();
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
    stream >> m_path;
    return true;
}

/*!
 * \brief RestSettings::save - сохранить настройки
 * \param name - имя раздела
 */
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
    stream << m_path;
    QSettings settings("JupiterSoft", "libSekura");
    settings.setValue(m_name, mCompress(array, false));
}

/*!
 * \brief RestSettings::path - возвращает путь
 * \return путь
 */
QString RestSettings::path() const { return m_path; }

/*!
 * \brief RestSettings::setPath - установить путь
 * \param newPath - новый путь
 */
void RestSettings::setPath(const QString &newPath) {
    if (m_path == newPath)
        return;
    m_path = newPath;
    emit pathChanged();
}

/*!
 * \brief RestSettings::setHeaderValue - установить значение заголовка
 * \param key - ключ заголовка
 * \param val - значение
 */
void RestSettings::setHeaderValue(const QString &key, const QByteArray &val) {
    bool changed = false;
    if (!m_headers.contains(key) && (m_headers[key] != val))
        changed = true;
    if (changed) {
        m_headers[key] = val;
        emit headerChanged(key);
    }
}

/*!
 * \brief RestSettings::setDataValue - установить значение данных, дополнительные данные, которые
 * сохраняются и передаются
 * \param key - ключ
 * \param val - значение
 */
void RestSettings::setDataValue(const QString &key, const QByteArray &val) { m_data[key] = val; }

/*!
 * \brief RestSettings::setHeaders - установить все заголовки
 * \param headers - заголовки
 */
void RestSettings::setHeaders(const QByteArrayMap &headers) {
    m_headers = headers;
    emit headerChanged("");
}

/*!
 * \brief RestSettings::setData - установить все данные
 * \param data - данные
 */
void RestSettings::setData(const QByteArrayMap &data) { m_data = data; }

/*!
 * \brief RestSettings::sslConfig - получить настройки SSL
 * \return указатель на настройки SSL
 */
QSslConfiguration *RestSettings::sslConfig() const { return m_sslConfig; }

/*!
 * \brief RestSettings::setSslConfig - установить настройки SSL
 * \param newSslConfig - указатель на новые настройки SSL
 */
void RestSettings::setSslConfig(QSslConfiguration *newSslConfig) {
    if (m_sslConfig == newSslConfig)
        return;
    m_sslConfig = newSslConfig;
    emit sslConfigChanged();
}

/*!
 * \brief RestSettings::mCompress - функция упаковки данных, может быть переопределена для
 * хранения в зашифрованном виде
 * \param arr - массив
 * \param direct - направление true сжатие/зашифровка, fals распаковка/расшифровка
 * \return возвращает обработанный массив
 */
QByteArray RestSettings::mCompress(const QByteArray &arr, bool direct) const {
    if (direct)
        return qUncompress(arr);
    return qCompress(arr);
}
