/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_COMBOBOX_H
#define SEKURA_COMBOBOX_H

#include "baseitem.h"
#include <QWidget>

namespace Sekura {

    namespace Ui {
        class ComboBox;
    }

    class ComboBox : public BaseItem {
        Q_OBJECT

      public:
        explicit ComboBox(int type, QWidget *parent = nullptr);
        ~ComboBox();

        void setValue(const QVariant &dt) override;
        QVariant value() const override;
        void setCaption(const QString &str) override;
        void setViewValue(const QVariant &) override{};

        void setModel(const QVariantList &list);

      protected slots:
        void on_comboBox_activated(int a);

      private:
        Ui::ComboBox *ui;
        QMap<QString, int> m_id2item;
        QMap<int, QString> m_item2id;
        QString m_code;
    };

} // namespace Sekura
#endif // SEKURA_COMBOBOX_H
