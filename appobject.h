/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_APPOBJECT_H
#define SEKURA_APPOBJECT_H

#include "basewidget.h"

#include <QObject>

namespace Sekura {

    class appObject : public QObject {
        Q_OBJECT
      public:
        explicit appObject(QObject *parent = nullptr);

        BaseWidget *loadFromFile(const QString &str);

      signals:

      public slots:
        void startWidget(Sekura::BaseWidget *ptr);
    };

} // namespace Sekura

#endif // SEKURA_APPOBJECT_H
