/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "menu.h"
#include "tablewidget.h"
#include "treewidget.h"

using namespace Sekura;

Menu::Menu(QMenuBar *mb, const RestSettings *settings, QObject *parent)
    : QObject{parent}, m_menuBar(mb), m_settings(settings) {
    m_menu = new TreeModel("a_menus", settings, this);
    connect(m_menu, &TreeModel::layoutChanged, this, &Menu::startCreateMenu);
}

void Menu::startCreateMenu() {
    QVariantMap map;
    m_menu->makeValues(map, QStringList() << "Name"
                                          << "Action");
    qDebug() << map;
    QVariantList lst0 = map["childs"].toList();
    foreach (QVariant v, lst0) {
        QVariantMap m = v.toMap();
        QMenu *menu = m_menuBar->addMenu(m["Name"].toString());
        appendChilds(menu, m["childs"].toList());
    }
}

void Menu::triggered() {
    QAction *action = qobject_cast<QAction *>(sender());
    if (m_actions.contains(action)) {
        QString val = m_actions[action];
        QStringList lst = val.split("::");
        if (lst[0] == "Sekura") {
            QVariantMap args;
            args["title"] = action->text();
            args["model"] = lst[2];
            if (lst[1] == "TableWidget") {
                TableWidget *widget = new TableWidget(args, m_settings);
                emit childCreated(widget);
            } else if (lst[1] == "TreeWidget") {
                TreeWidget *widget = new TreeWidget(args, m_settings);
                emit childCreated(widget);
            }
        } else {
            emit createCustomChild(action->text(), val);
        }
    }
}

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
