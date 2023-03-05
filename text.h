/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_TEXT_H
#define SEKURA_TEXT_H

#include "baseitem.h"

#include <QTextDocument>
#include <QWidget>

namespace Sekura {

    namespace Ui {
        class Text;
    }

    class Text : public BaseItem {
        Q_OBJECT

      public:
        explicit Text(int type = 0, QWidget *parent = nullptr);
        ~Text();

        void setValue(const QVariant &dt) override;
        QVariant value() const override;
        void setCaption(const QString &str) override;
        void setViewValue(const QVariant &) override;

        void setReadOnly() override;

      protected slots:
        void bold();

      private:
        Ui::Text *ui;
        QTextDocument *m_document;
    };

} // namespace Sekura
#endif // SEKURA_TEXT_H
