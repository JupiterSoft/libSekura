/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_ELEMENT_H
#define SEKURA_ELEMENT_H

#include "baseitem.h"
#include <QWidget>

namespace Sekura {

    namespace Ui {
        class Element;
    }

    class Element : public BaseItem {
        Q_OBJECT

      public:
        explicit Element(int type, QWidget *parent = nullptr);
        ~Element();

        void setCaption(const QString &str) override;
        void setValue(const QVariant &val) override;
        QVariant value() const override;
        void setViewValue(const QVariant &val) override;
        void setReadOnly() override;

      protected slots:
        void cmb_indexChanged(int a);
        void sb_valueChanged(int a);

      private:
        Ui::Element *ui;
    };

} // namespace Sekura
#endif // SEKURA_ELEMENT_H
