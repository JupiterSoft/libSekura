/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_BOTTOMTABLEWIDGET_H
#define SEKURA_BOTTOMTABLEWIDGET_H

#include <QWidget>

namespace Sekura {

    namespace Ui {
        class BottomTableWidget;
    }

    class BottomTableWidget : public QWidget {
        Q_OBJECT

      public:
        explicit BottomTableWidget(QWidget *parent = nullptr);
        ~BottomTableWidget();

      signals:
        void createTable();
        void dropTable();

      private:
        Ui::BottomTableWidget *ui;
    };

} // namespace Sekura
#endif // SEKURA_BOTTOMTABLEWIDGET_H
