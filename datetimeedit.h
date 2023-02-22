/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_DATETIMEEDIT_H
#define SEKURA_DATETIMEEDIT_H

#include "baseitem.h"
#include <QWidget>

namespace Sekura {

    namespace Ui {
        class DateTimeEdit;
    }

    class DateTimeEdit : public BaseItem {
        Q_OBJECT

      public:
        explicit DateTimeEdit(int type = 0, QWidget *parent = nullptr);
        ~DateTimeEdit();

        void setValue(const QVariant &dt) override;
        QVariant value() const override;
        void setCaption(const QString &str) override;

      private:
        Ui::DateTimeEdit *ui;
    };

} // namespace Sekura
#endif // SEKURA_DATETIMEEDIT_H