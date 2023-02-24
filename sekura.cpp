/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "sekura.h"
#include "checkbox.h"
#include "combobox.h"
#include "datetimeedit.h"
#include "itemwidget.h"
#include "lineedit.h"
#include "menu.h"
#include "spinbox.h"
#include "tablewidget.h"
#include "treewidget.h"

#include <QCryptographicHash>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUuid>

using namespace Sekura;

QString Sekura::Interface::genUFID() {
    return QCryptographicHash::hash(QUuid::createUuid().toString().toUtf8(),
                                    QCryptographicHash::Sha256)
        .toBase64(QByteArray::Base64UrlEncoding);
}

QString Sekura::Interface::genKey() {
    QCryptographicHash hash(QCryptographicHash::Sha256);
    hash.addData(QUuid::createUuid().toByteArray());
    hash.addData(QDateTime::currentDateTimeUtc().toString().toUtf8());
    QByteArray gen = hash.result();
    QByteArray m_key(gen.size() / 2, 0);
    for (int i = 0; i < m_key.size(); i++) {
        m_key[i] = gen[i * 2] ^ gen[i * 2 + 1];
    }
    return m_key.toBase64(QByteArray::Base64UrlEncoding | QByteArray::OmitTrailingEquals);
}

BaseItem *Interface::createItem(const QVariantMap &m, QWidget *parent) {
    BaseItem *ptr = nullptr;
    QString type = m["ui_desk"].toString();
    if (type.left(8) == "LineEdit") {
        int t = 0;
        QString str = type.split(";")[0].remove("LineEdit").remove("(").remove(")");
        bool ok;
        t = str.toInt(&ok);
        if (!ok)
            t = 0;
        LineEdit *le = new LineEdit(t, parent); ///<<вставить инициализацию
        if (m.contains("fk_table")) {
            le->setTable(m["fk_table"].toString());
        }
        ptr = le;
    } else if (type.left(12) == "DateTimeEdit") {
        ptr = new DateTimeEdit(0, parent);
    } else if (type.left(8) == "ComboBox") {
        ptr = new ComboBox(0, parent);
    } else if (type.left(8) == "CheckBox") {
        ptr = new CheckBox(0, parent);
    } else if (type.left(7) == "SpinBox") {
        SpinBox *sb = new SpinBox(0, parent);
        int min, max;
        QStringList lst = type.split(";")[0].remove("SpinBox").remove("(").remove(")").split(",");
        bool ok;
        min = lst[1].toInt(&ok);
        if (!ok)
            min = 0;
        max = lst[2].toInt(&ok);
        if (!ok)
            max = 0;
        sb->setMinMax(min, max);
        ptr = sb;
    }
    return ptr;
}

Menu *Interface::createMenu(QMenuBar *mb, const RestSettings *settings, QObject *parent) {
    static Menu *menu = nullptr;
    if (menu == nullptr)
        menu = new Menu(mb, settings, parent);
    return menu;
}

BaseWidget *privateParseWidgets(const QVariantMap &desc, const RestSettings *settings,
                                const QVariantMap &map, QWidget *parent) {
    BaseWidget *ret = nullptr;
    QVariantMap temp = map;
    QString str = desc["type"].toString();
    if ((str == "Table") || (str == "Tree") || (str == "Item")) {
        temp["model"] = desc["model"];
        QVariantMap filter;
        if (temp.contains("filter"))
            filter = temp["filter"].toMap();
        if (desc.contains("filter")) {
            foreach (QVariant f, desc["filter"].toList()) {
                QString str = f.toString();
                QStringList sl = str.split("=");
                QString key = sl[0].trimmed();
                if (!filter.contains(key)) {
                    if (key != "id")
                        filter[key] = "";
                }
            }
        }
        temp["filter"] = filter;
        if (str == "Table") {
            ret = new TableWidget(temp, settings, parent);
        } else if (str == "Tree") {
            ret = new TreeWidget(temp, settings, parent);
        } else if (str == "Item") {
            ret = new ItemWidget(temp, settings, parent);
        }
    } else if (str == "Box") {

    } else if (str == "Splitter") {

    } else if (str == "Scroll") {
    }
    return ret;
}

BaseWidget *Interface::createWidget(const QString &desc, const RestSettings *settings,
                                    const QVariantMap &map, QWidget *parent) {
    QVariantMap vals = QJsonDocument::fromJson(desc.toUtf8()).object().toVariantMap();

    return privateParseWidgets(vals, settings, map, parent);
}

void sekura_init_resources() { Q_INIT_RESOURCE(resources); }
