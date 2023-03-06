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
        explicit appObject(const QString &str, QObject *parent = nullptr);

        BaseWidget *loadFromFile(const QString &str);

        bool isError() const { return m_error; }

      signals:

      public slots:
        void startWidget(Sekura::BaseWidget *ptr);

      private:
        QString m_conf;
        RestSettings *m_settings;
        bool m_error;
    };

} // namespace Sekura

#endif // SEKURA_APPOBJECT_H
