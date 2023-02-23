/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_SPINBOX_H
#define SEKURA_SPINBOX_H

#include "baseitem.h"
#include <QWidget>

namespace Sekura {

    namespace Ui {
        class SpinBox;
    }

    class SpinBox : public BaseItem {
        Q_OBJECT

      public:
        explicit SpinBox(int type = 0, QWidget *parent = nullptr);
        ~SpinBox();

        void setMinMax(int min, int max);
        void setValue(const QVariant &val) override;
        void setCaption(const QString &str) override;
        QVariant value() const override;
        void setViewValue(const QVariant &) override{};

      private:
        Ui::SpinBox *ui;
    };

} // namespace Sekura
#endif // SEKURA_SPINBOX_H
