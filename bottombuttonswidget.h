/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_BOTTOMBUTTONSWIDGET_H
#define SEKURA_BOTTOMBUTTONSWIDGET_H

#include <QWidget>

namespace Sekura {

    namespace Ui {
        class BottomButtonsWidget;
    }

    class BottomButtonsWidget : public QWidget {
        Q_OBJECT

      public:
        explicit BottomButtonsWidget(QWidget *parent = nullptr);
        ~BottomButtonsWidget();

      signals:
        void saveForm();
        void closeForm();

      private:
        Ui::BottomButtonsWidget *ui;
    };

} // namespace Sekura
#endif // SEKURA_BOTTOMBUTTONSWIDGET_H
