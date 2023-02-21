/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_H
#define SEKURA_H

#include <QMap>

namespace Sekura {

    class RestSettings;
    class RestClient;
    class AuthDialog;
    class AbstractTableModel;
    class TableModel;
    class LineEdit;
    class DateTimeEdit;
    class ComboBox;

    typedef QMap<QString, QByteArray> QByteArrayMap;

    class Interface {
      public:
        static QString genUFID();
        static QString genKey();
    };

}; // namespace Sekura

#endif // SEKURA_H
