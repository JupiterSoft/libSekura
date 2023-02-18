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

    typedef QMap<QString, QByteArray> QByteArrayMap;

    class Interface {
      public:
        static QString genUFID();
    };

}; // namespace Sekura

#endif // SEKURA_H
