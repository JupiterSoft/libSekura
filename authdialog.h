/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef AUTHDIALOG_H
#define AUTHDIALOG_H

#include "sekura.h"
#include <QDialog>

namespace Sekura {

    namespace Ui {
        class AuthDialog;
    }

    /*!
     * \brief The AuthDialog class - диалоговое окно авторизации по ключу
     */
    class AuthDialog : public QDialog {
        Q_OBJECT

      public:
        explicit AuthDialog(RestSettings *settings, bool requestKeyButton = false,
                            QWidget *parent = nullptr);
        ~AuthDialog();

      public slots:
        virtual void accept();

        void success(const QJsonObject &);
        void error(const QJsonObject &);

        void on_pbRequestKey_clicked();

      private:
        Ui::AuthDialog *ui;
        RestSettings *m_settings;
        RestClient *m_client;
    };

} // namespace Sekura

#endif // AUTHDIALOG_H
