/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_H
#define SEKURA_H

#include <QMap>
#include <QWidget>

namespace Sekura {

    class RestSettings;
    class RestClient;
    class AuthDialog;
    class TableModel;
    class BaseItem;
    class LineEdit;
    class DateTimeEdit;
    class ComboBox;
    class SpinBox;
    class TableWidget;
    class ItemWidget;
    class ItemModel;

    typedef QMap<QString, QByteArray> QByteArrayMap;

    class Interface {
      public:
        static QString genUFID();
        static QString genKey();
        static BaseItem *createItem(const QVariantMap &element, QWidget *parent);
    };

}; // namespace Sekura

void sekura_init_resources();

#endif // SEKURA_H
