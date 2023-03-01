/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_BASEITEM_H
#define SEKURA_BASEITEM_H

#include <QWidget>

namespace Sekura {

    /*!
     * \brief The BaseItem class - базовый абстрактный класс для всех элементов
     */
    class BaseItem : public QWidget {
        Q_OBJECT
      public:
        explicit BaseItem(int type = 0, QWidget *parent = nullptr);

        virtual void setCaption(const QString &) = 0;
        virtual void setValue(const QVariant &) = 0;
        virtual QVariant value() const = 0;
        virtual void setViewValue(const QVariant &) = 0;
        virtual void setReadOnly() = 0;

      signals:
        void valueChanged(QVariant);

      protected:
        int m_type;
    };

} // namespace Sekura

#endif // SEKURA_BASEITEM_H
