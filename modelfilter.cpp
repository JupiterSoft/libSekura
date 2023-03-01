/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "modelfilter.h"

namespace Sekura {

    ModelFilter::ModelFilter(QObject *parent) : QObject{parent} { m_ref = 0; }

    const QVariantMap ModelFilter::value(const QString &index) const {
        return m_filter[index].toMap();
    }

    const QVariant ModelFilter::value(const QString &table, const QString &index) const {
        return m_filter[table].toMap()[index];
    }

    void ModelFilter::setValue(const QString &index, const QVariantMap &value) {
        m_filter[index] = value;
        emit itemChanged(index, value);
    }

    void ModelFilter::setValue(const QString &table, const QString &index, const QVariant &val) {
        QVariantMap m;
        if (m_filter.contains(table))
            m = m_filter[table].toMap();
        m[index] = val;
        m_filter[table] = m;
        emit itemChanged(table, m);
    }

    const QVariantMap &ModelFilter::values() const { return m_filter; }

    bool ModelFilter::contains(const QString &table) const { return m_filter.contains(table); }

    bool ModelFilter::contains(const QString &table, const QString &index) const {
        return m_filter.contains(table) && m_filter[table].toMap().contains(index);
    }

    void ModelFilter::remove(const QString &table) { m_filter.remove(table); }

} // namespace Sekura
