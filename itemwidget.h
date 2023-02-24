/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_ITEMWIDGET_H
#define SEKURA_ITEMWIDGET_H

#include "basewidget.h"
#include "itemmodel.h"
#include "sekura.h"
#include <QWidget>

namespace Sekura {

    namespace Ui {
        class ItemWidget;
    }

    class ItemWidget : public BaseWidget {
        Q_OBJECT

      public:
        explicit ItemWidget(const QVariantMap &map, const RestSettings *settings,
                            QWidget *parent = nullptr);
        ~ItemWidget();

      protected slots:
        void connectInterface(const QVariant &val);

        void saveForm();
        void closeForm();

      signals:

      protected:
        void reload();

      private:
        Ui::ItemWidget *ui;
        ItemModel *m_model;
        const RestSettings *m_settings;
    };

} // namespace Sekura
#endif // SEKURA_ITEMWIDGET_H
