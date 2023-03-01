/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "basewidget.h"
#include "modelfilter.h"

namespace Sekura {

    BaseWidget::BaseWidget(ModelFilter *filter, QWidget *parent)
        : QWidget{parent}, m_mainForm(false) {
        if (filter == nullptr) {
            m_modelFilter = new ModelFilter;
        } else {
            m_modelFilter = filter;
        }
        m_modelFilter->addRef();
    }

    BaseWidget::~BaseWidget() {
        if (m_modelFilter->decRef() <= 0)
            delete m_modelFilter;
    }

    void BaseWidget::setFilter(ModelFilter *filter) {
        m_modelFilter = filter;
        m_modelFilter->addRef();
    }

    //    void BaseWidget::changeId(const QString &table, const QString &id) {
    //        Q_UNUSED(table)
    //        Q_UNUSED(id)
    //    }

} // namespace Sekura
