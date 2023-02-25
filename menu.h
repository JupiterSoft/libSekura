/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_MENU_H
#define SEKURA_MENU_H

#include "basewidget.h"
#include "treemodel.h"

#include <QMenuBar>
#include <QObject>

namespace Sekura {

    /*!
     * \brief The Menu class - меню
     */
    class Menu : public QObject {
        Q_OBJECT
      public:
        explicit Menu(QMenuBar *mb, const RestSettings *settings, QObject *parent = nullptr);

      signals:
        void childCreated(Sekura::BaseWidget *);
        void createCustomChild(const QString &, const QString &);

      protected slots:
        void startCreateMenu();
        void triggered();

      protected:
        void appendChilds(QMenu *menu, const QVariantList &childs);

      private:
        QMenuBar *m_menuBar;
        TreeModel *m_menu;
        QMap<QAction *, QString> m_actions;
        const RestSettings *m_settings;
    };

} // namespace Sekura

#endif // SEKURA_MENU_H
