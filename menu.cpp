/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "menu.h"
#include "modelfilter.h"

using namespace Sekura;

/*!
 * \brief Menu::Menu - конструктор меню
 * \param mb - панель приложения
 * \param settings - установки соединения
 * \param parent - родительский объект
 */
Menu::Menu(QMenuBar *mb, QObject *parent) : QObject{parent}, m_menuBar(mb) {
    ModelFilter *filter = new ModelFilter(this);
    filter->setValue("temp", "model", "a_menus");
    filter->setValue("temp", "onlyMy", true);
    m_menu = new TreeModel(filter, this);
    connect(m_menu, &TreeModel::layoutChanged, this, &Menu::startCreateMenu);
}

/*!
 * \brief Menu::startCreateMenu - создание меню
 */
void Menu::startCreateMenu() {
    QVariantMap map;
    m_menu->makeValues(map, QStringList() << "Name"
                                          << "Action");
    // qDebug() << map;
    QVariantList lst0 = map["childs"].toList();
    foreach (QVariant v, lst0) {
        QVariantMap m = v.toMap();
        QMenu *menu = m_menuBar->addMenu(m["Name"].toString());
        appendChilds(menu, m["childs"].toList());
    }
}

/*!
 * \brief Menu::triggered - обработчик события активации элемента меню
 */
void Menu::triggered() {
    QAction *action = qobject_cast<QAction *>(sender());
    if (m_actions.contains(action)) {
        QString val = m_actions[action];
        BaseWidget *widget = Interface::createWidget(nullptr, val);
        widget->setWindowTitle(action->text());
        if (widget != nullptr) {
            emit childCreated(widget);
        } else {
            emit createCustomChild(action->text(), val);
        }
    }
}

/*!
 * \brief Menu::appendChilds - рекурсиваня функция добавления меню
 * \param menu - верхнее меню
 * \param childs - список подчиненных
 */
void Menu::appendChilds(QMenu *menu, const QVariantList &childs) {
    foreach (QVariant v, childs) {
        QVariantMap map = v.toMap();
        if (!map["childs"].toList().isEmpty()) {
            QMenu *m = menu->addMenu(map["Name"].toString());
            appendChilds(m, map["childs"].toList());
        } else {
            QAction *action = new QAction(map["Name"].toString());
            m_actions[action] = map["Action"].toString();
            connect(action, &QAction::triggered, this, &Menu::triggered);
            menu->addAction(action);
        }
    }
}
