/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_BASEWIDGET_H
#define SEKURA_BASEWIDGET_H

#include <QWidget>

namespace Sekura {

    class BaseWidget : public QWidget {
        Q_OBJECT
      public:
        explicit BaseWidget(QWidget *parent = nullptr);

      signals:
        void appendWidget(QWidget *);
        void parentReload();
    };

} // namespace Sekura

#endif // SEKURA_BASEWIDGET_H
