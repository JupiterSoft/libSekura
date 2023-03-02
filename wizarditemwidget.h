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
        void pbDel_clicked();
        void cmbItem_currentChanged(int a);
        void cmbModel_currentChanged(int a);

        void tree_itemSelectionChanged();
        void tw_itemChanged(QTableWidgetItem *item);

        void success(const QJsonObject &obj);
        void error(const QJsonObject &obj);

      protected:
        void parseJson();
        void toJson();

        void parse(const QVariantMap &map, QTreeWidgetItem *parent);
        void saveItem(QTreeWidgetItem *item);
        void fillItem(QTreeWidgetItem *item);

        QVariantMap mapItem(QTreeWidgetItem *item);
        void removeFromItem(QTreeWidgetItem *item);

      private:
        Ui::WizardItemWidget *ui;
        QStringList m_tables;
        QStringList m_idTables;
        QMap<QString, QStringList> m_fields;
        QMap<QString, QStringList> m_idFields;
        QMap<QTreeWidgetItem *, QVariantMap> m_items;

        QString m_form;
        QVariantMap m_data;
        RestClient *m_client;
        QTreeWidgetItem *m_currentItem;
    };

} // namespace Sekura
#endif // SEKURA_WIZARDITEMWIDGET_H
