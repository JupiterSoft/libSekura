/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_DATATYPE_H
#define SEKURA_DATATYPE_H

#include "baseitem.h"
#include <QWidget>

namespace Sekura {

    namespace Ui {
        class DataType;
    }

    class DataType : public BaseItem {
        Q_OBJECT

      public:
        explicit DataType(int type = 0, QWidget *parent = nullptr);
        ~DataType();

        void setCaption(const QString &str) override;
        void setValue(const QVariant &val) override;
        QVariant value() const override;
        void setViewValue(const QVariant &val) override;
        void setReadOnly() override;

      protected slots:
        void cmb_indexChanged(int a);
        void cb_check(int a);

      private:
        Ui::DataType *ui;
        QString m_data;
    };

} // namespace Sekura
#endif // SEKURA_DATATYPE_H
