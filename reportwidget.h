/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_REPORT_H
#define SEKURA_REPORT_H

#include "basewidget.h"

#include <QWebEngineView>
#include <QWidget>

namespace Sekura {

    namespace Ui {
        class ReportWidget;
    }

    class ReportWidget : public BaseWidget {
        Q_OBJECT

      public:
        explicit ReportWidget(ModelFilter *filter, QWidget *parent = nullptr);
        ~ReportWidget();

      public slots:
        void reload();

      protected slots:
        void success(const QJsonObject &obj);
        void error(const QJsonObject &obj);

      private:
        Ui::ReportWidget *ui;
        RestClient *m_client;
        QVariantMap m_report;
        QWebEngineView *m_view;
    };

} // namespace Sekura
#endif // SEKURA_REPORT_H
