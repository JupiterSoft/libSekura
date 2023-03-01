/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_TREEWIDGET_H
#define SEKURA_TREEWIDGET_H

#include "basewidget.h"
#include "restsettings.h"
#include "treemodel.h"

#include <QWidget>

namespace Sekura {

    namespace Ui {
        class TreeWidget;
    }

    /*!
     * \brief The TreeWidget class - форма отображения данных в виде дерева
     */
    class TreeWidget : public BaseWidget {
        Q_OBJECT

      public:
        explicit TreeWidget(ModelFilter *filter, const RestSettings *settings,
                            QWidget *parent = nullptr);
        ~TreeWidget();

      signals:

      public slots:
        void on_pbAdd_clicked();
        void on_pbEdit_clicked();
        void on_pbDel_clicked();

        void on_treeView_clicked(const QModelIndex &index);
        void on_treeView_doubleClicked(const QModelIndex &index);

        // void changeId(const QString &table, const QString &id) override;

      protected:
        void openOnEdit(const QModelIndex &index = QModelIndex());

      private:
        Ui::TreeWidget *ui;
        const RestSettings *m_settings;
        // QVariantMap m_data;
        TreeModel *m_model;
    };

} // namespace Sekura
#endif // SEKURA_TREEWIDGET_H
