/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#ifndef SEKURA_H
#define SEKURA_H

#include <QMap>
#include <QMenuBar>
#include <QWidget>

namespace Sekura {

    class RestSettings;
    class RestClient;
    class AuthDialog;
    class TableModel;
    class BaseItem;
    class BaseWidget;
    class LineEdit;
    class DateTimeEdit;
    class ComboBox;
    class SpinBox;
    class TableWidget;
    class ItemWidget;
    class ItemModel;
    class Menu;

    typedef QMap<QString, QByteArray> QByteArrayMap;

    /*!
     * \brief The Interface class - интерфейс со статическими функциями
     */
    class Interface {
      public:
        static QString genUFID();
        static QString genKey();
        static BaseItem *createItem(const QVariantMap &element, QWidget *parent);
        static Menu *createMenu(QMenuBar *mb, const RestSettings *settings, QObject *parent);
        static BaseWidget *createWidget(const QString &desc, const RestSettings *settings,
                                        const QVariantMap &map = QVariantMap(),
                                        QWidget *parent = nullptr);
    };

}; // namespace Sekura

void sekura_init_resources();

#endif // SEKURA_H
