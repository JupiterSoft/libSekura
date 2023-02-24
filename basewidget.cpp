/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "basewidget.h"

namespace Sekura {

    BaseWidget::BaseWidget(QWidget *parent) : QWidget{parent}, m_mainForm(false) {}

    void BaseWidget::changeId(const QString &table, const QString &id) {
        emit idChanged(table, id);
    }

} // namespace Sekura
