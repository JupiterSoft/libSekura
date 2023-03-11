/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "appobject.h"

#include <QFile>

namespace Sekura {

    appObject::appObject(QObject *parent) : QObject{parent} {}

    BaseWidget *appObject::loadFromFile(const QString &str) {
        QFile file(str);
        if (!file.open(QIODevice::ReadOnly))
            return nullptr;
        return Interface::createWidget(nullptr, file.readAll());
    }

    void appObject::startWidget(BaseWidget *ptr) {
        ptr->setParent(nullptr);
        ptr->resize(800, 600);
        connect(ptr, &BaseWidget::appendWidget, this, &appObject::startWidget);
        ptr->show();
    }

} // namespace Sekura
