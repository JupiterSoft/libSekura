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

    /*!
     * \brief The ItemWidget class форма для отображения элемента данных
     */
    class ItemWidget : public BaseWidget {
        Q_OBJECT

      public:
        explicit ItemWidget(ModelFilter *filter, QWidget *parent = nullptr);
        ~ItemWidget();

      public slots:
        // void changeId(const QString &table, const QString &id) override;

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
    };

} // namespace Sekura
#endif // SEKURA_ITEMWIDGET_H
