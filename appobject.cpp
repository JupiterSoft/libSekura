/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "appobject.h"
#include "restsettings.h"

#include <QFile>

namespace Sekura {

    appObject::appObject(const QString &str, QObject *parent) : QObject{parent}, m_conf(str) {
        m_settings = new RestSettings(this);
        if (!m_settings->load(m_conf)) {
            m_error = true;
        }
        Sekura::Interface::setSettings(m_settings);
    }

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
