/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_LISTWIDGET_H
#define SEKURA_LISTWIDGET_H

#include "basewidget.h"
#include "listmodel.h"
#include <QWidget>

namespace Sekura {

    namespace Ui {
        class ListWidget;
    }

    class ListWidget : public BaseWidget {
        Q_OBJECT

      public:
        explicit ListWidget(ModelFilter *filter, QWidget *parent = nullptr);
        ~ListWidget();

      protected slots:
        void cmb_currentChanged(int a);
        void model_initialized();

      private:
        Ui::ListWidget *ui;
        ListModel *m_model;
    };

} // namespace Sekura
#endif // SEKURA_LISTWIDGET_H
