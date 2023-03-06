/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_MAINWINDOW_H
#define SEKURA_MAINWINDOW_H

#include "basewidget.h"

#include <QMainWindow>

namespace Sekura {

    namespace Ui {
        class MainWindow;
    }

    class MainWindow : public QMainWindow {
        Q_OBJECT

      public:
        explicit MainWindow(const QString &conf, QWidget *parent = nullptr);
        ~MainWindow();

        bool isError() const { return m_error; }

      public slots:
        void appendWidget(Sekura::BaseWidget *widget);

      private:
        Ui::MainWindow *ui;

        QString m_conf;
        RestSettings *m_settings;
        bool m_error;
    };

} // namespace Sekura
#endif // SEKURA_MAINWINDOW_H
