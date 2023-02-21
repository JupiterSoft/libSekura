/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_SPINBOX_H
#define SEKURA_SPINBOX_H

#include <QWidget>

namespace Sekura {

    namespace Ui {
        class SpinBox;
    }

    class SpinBox : public QWidget {
        Q_OBJECT

      public:
        explicit SpinBox(QWidget *parent = nullptr);
        ~SpinBox();

        void setMinMax(int min, int max);
        void setValue(int val);
        void setCaption(const QString &str);
        int value() const;

      private:
        Ui::SpinBox *ui;
    };

} // namespace Sekura
#endif // SEKURA_SPINBOX_H
