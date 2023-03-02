/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_WIZARDITEMWIDGET_H
#define SEKURA_WIZARDITEMWIDGET_H

#include "restclient.h"

#include <QDialog>
#include <QTableWidgetItem>
#include <QTreeWidgetItem>

namespace Sekura {

    namespace Ui {
        class WizardItemWidget;
    }

    class WizardItemWidget : public QDialog {
        Q_OBJECT

      public:
        explicit WizardItemWidget(const QString &form, QWidget *parent = nullptr);
        ~WizardItemWidget();

        const QString &form() const { return m_form; }

      protected slots:
        void pbSave_clicked();
        void pbAdd_clicked();
        void pbAddChild_clicked();
        void pbMove_clicked();
        void pbDel_clicked();
        void cmbItem_currentChanged(int a);
        void cmbModel_currentChanged(int a);

        void tree_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
        void tw_itemChanged(QTableWidgetItem *item);

      private:
        Ui::WizardItemWidget *ui;

        QString m_form;
        RestClient *m_client;
    };

} // namespace Sekura
#endif // SEKURA_WIZARDITEMWIDGET_H
