/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_DATETIMEEDIT_H
#define SEKURA_DATETIMEEDIT_H

#include <QWidget>

namespace Sekura {

    namespace Ui {
        class DateTimeEdit;
    }

    class DateTimeEdit : public QWidget {
        Q_OBJECT

      public:
        explicit DateTimeEdit(QWidget *parent = nullptr);
        ~DateTimeEdit();

        void setValue(const QDateTime &dt);
        QDateTime value() const;
        void setCaption(const QString &str);

      private:
        Ui::DateTimeEdit *ui;
    };

} // namespace Sekura
#endif // SEKURA_DATETIMEEDIT_H
