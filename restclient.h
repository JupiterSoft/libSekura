/*
 * © 2022
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */

#ifndef RESTCLIENT_H
#define RESTCLIENT_H

#include <QObject>

namespace Sekura {

    class RestClient : public QObject {
        Q_OBJECT
      public:
        explicit RestClient(QObject *parent = 0);
    };

} // namespace Sekura

#endif // RESTCLIENT_H
