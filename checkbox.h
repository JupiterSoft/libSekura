/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_CHECKBOX_H
#define SEKURA_CHECKBOX_H

#include "baseitem.h"
#include <QWidget>

namespace Sekura {

    namespace Ui {
        class CheckBox;
    }

    class CheckBox : public BaseItem {
        Q_OBJECT

      public:
        explicit CheckBox(int type = 0, QWidget *parent = nullptr);
        ~CheckBox();

        void setValue(const QVariant &dt) override;
        QVariant value() const override;
        void setCaption(const QString &str) override;

      private:
        Ui::CheckBox *ui;
    };

} // namespace Sekura
#endif // SEKURA_CHECKBOX_H