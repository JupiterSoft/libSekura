/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_FOREIGNKEY_H
#define SEKURA_FOREIGNKEY_H

#include "baseitem.h"
#include "restclient.h"
#include <QWidget>

namespace Sekura {

    namespace Ui {
        class ForeignKey;
    }

    class ForeignKey : public BaseItem {
        Q_OBJECT

      public:
        explicit ForeignKey(int type, QWidget *parent = nullptr);
        ~ForeignKey();

        void setCaption(const QString &str) override;
        void setValue(const QVariant &val) override;
        QVariant value() const override;
        void setViewValue(const QVariant &val) override;
        void setReadOnly() override;

      signals:
        void valueTable(const QString &);
        void valueField(const QString &);

      public slots:
        void setValueFromParent(const QString &v);

      protected slots:
        void success(const QJsonObject &obj);
        void error(const QJsonObject &obj);
        void cmb_indexChanged(int a);

      private:
        Ui::ForeignKey *ui;
        RestClient *m_client;
        QString m_value;
        QStringList m_fields;
        QStringList m_tables;
        QStringList m_list;
        bool m_initialized;
    };

} // namespace Sekura
#endif // SEKURA_FOREIGNKEY_H
