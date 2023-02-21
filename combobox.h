/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_COMBOBOX_H
#define SEKURA_COMBOBOX_H

#include <QWidget>

namespace Sekura {

    namespace Ui {
        class ComboBox;
    }

    class ComboBox : public QWidget {
        Q_OBJECT

      public:
        explicit ComboBox(QWidget *parent = nullptr);
        ~ComboBox();

        void setModel(const QVariantList &list);
        void setCurrentId(const QString &id);
        void setCaption(const QString &str);
        QString currentId() const;

      protected slots:
        void on_comboBox_activated(int a);

      signals:
        void valueChanged(const QString &id);

      private:
        Ui::ComboBox *ui;
        QMap<QString, int> m_id2item;
        QMap<int, QString> m_item2id;
    };

} // namespace Sekura
#endif // SEKURA_COMBOBOX_H
