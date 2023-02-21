/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "baseitem.h"

namespace Sekura {

    BaseItem::BaseItem(int type, QWidget *parent) : QWidget{parent} { m_type = type; }

} // namespace Sekura
