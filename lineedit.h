/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_LINEEDIT_H
#define SEKURA_LINEEDIT_H

#include <QWidget>

namespace Sekura {

    namespace Ui {
        class LineEdit;
    }

    class LineEdit : public QWidget {
        Q_OBJECT

      public:
        explicit LineEdit(int type = 0, QWidget *parent = nullptr);
        ~LineEdit();

        void setText(const QString &str);
        QString text() const;
        void setCaption(const QString &str);
        void setBlock(bool t);

      signals:
        void buttonClicked();

      private:
        Ui::LineEdit *ui;
        int m_type;
    };

} // namespace Sekura
#endif // SEKURA_LINEEDIT_H
