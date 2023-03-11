/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SYSTEMCOMMANDFORM_H
#define SYSTEMCOMMANDFORM_H

#include "restclient.h"
#include "restsettings.h"
#include <QWidget>

namespace Sekura {

    namespace Ui {
        class SystemCommandWidget;
    }

    class SystemCommandWidget : public QWidget {
        Q_OBJECT

      public:
        explicit SystemCommandWidget(const QString &sys, QWidget *parent = nullptr);
        ~SystemCommandWidget();

      public slots:

        void success(const QJsonObject &obj);
        void error(const QJsonObject &obj);
        void pbCommands();
        void pbSend();

      private:
        Ui::SystemCommandWidget *ui;
        Sekura::RestSettings *m_settings;
        Sekura::RestClient *m_client;
        QString m_systemKey;
        QString m_fileName;
    };

} // namespace Sekura

#endif // SYSTEMCOMMANDFORM_H
