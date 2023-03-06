/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_AUTHWIDGET_H
#define SEKURA_AUTHWIDGET_H

#include "restsettings.h"

#include <QWidget>

namespace Sekura {

    namespace Ui {
        class AuthWidget;
    }

    class AuthWidget : public QWidget {
        Q_OBJECT

      public:
        explicit AuthWidget(QWidget *parent = nullptr);
        ~AuthWidget();

      public slots:
        void cmb_index(int a);
        void pb_save();
        void pb_auth();
        void pb_del();

      private:
        Ui::AuthWidget *ui;
        RestSettings m_settings;

        void reload();
    };

} // namespace Sekura
#endif // SEKURA_AUTHWIDGET_H
