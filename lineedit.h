/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_LINEEDIT_H
#define SEKURA_LINEEDIT_H

#include "baseitem.h"
#include <QWidget>

namespace Sekura {

    namespace Ui {
        class LineEdit;
    }

    class LineEdit : public BaseItem {
        Q_OBJECT

      public:
        explicit LineEdit(int type = 0, QWidget *parent = nullptr);
        ~LineEdit();

        void setValue(const QVariant &val) override;
        QVariant value() const override;
        void setCaption(const QString &str) override;

      protected:
        void setBlock(bool t);

      private:
        Ui::LineEdit *ui;
    };

} // namespace Sekura
#endif // SEKURA_LINEEDIT_H
