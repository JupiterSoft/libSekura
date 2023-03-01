/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_MODELFILTER_H
#define SEKURA_MODELFILTER_H

#include <QObject>
#include <QVariantMap>

namespace Sekura {

    class ModelFilter : public QObject {
        Q_OBJECT
      public:
        explicit ModelFilter(QObject *parent = nullptr);

        const QVariantMap value(const QString &table) const;
        const QVariant value(const QString &table, const QString &index) const;
        void setValue(const QString &table, const QVariantMap &value);
        void setValue(const QString &table, const QString &index, const QVariant &val);
        const QVariantMap &values() const;
        bool contains(const QString &table) const;
        bool contains(const QString &table, const QString &index) const;
        void remove(const QString &table);

        int decRef() { return --m_ref; }
        int addRef() { return ++m_ref; }

        int ref() const { return m_ref; }

      signals:
        void itemChanged(const QString &table, const QVariantMap &value);

      protected:
        QVariantMap m_filter;
        int m_ref;
    };

} // namespace Sekura

#endif // SEKURA_MODELFILTER_H
