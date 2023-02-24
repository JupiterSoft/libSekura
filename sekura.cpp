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
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QScrollArea>
#include <QSplitter>
#include <QTimer>
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

BaseWidget *privateParseWidgets(QVariantMap &ref, const QVariantMap &desc,
                                const RestSettings *settings, const QVariantMap &map,
                                QWidget *parent) {
    BaseWidget *ret = nullptr;
    QVariantMap temp = map;
    QString str = desc["type"].toString();
    if ((str == "Table") || (str == "Tree") || (str == "Item")) {
        temp["model"] = desc["model"];
        QVariantMap filter, f1;
        if (temp.contains("filter"))
            f1 = temp["filter"].toMap();
        if (desc.contains("filter")) {
            foreach (QVariant f, desc["filter"].toList()) {
                QString str = f.toString();
                QStringList sl = str.split("=");
                QString key = sl[0].trimmed();
                QString val = sl[1].trimmed();
                if (ref.contains(val)) {
                    filter[key] = ref[val];
                } else if (f1.contains(key)) {
                    filter[key] = f1[key];
                }
            }
        }
        temp["filter"] = filter;
        if (filter.contains("id")) {
            ref[temp["model"].toString() + ".id"] = filter["id"];
        }
        if (str == "Table") {
            ret = new TableWidget(temp, settings, parent);
        } else if (str == "Tree") {
            ret = new TreeWidget(temp, settings, parent);
        } else if (str == "Item") {
            ret = new ItemWidget(temp, settings, parent);
        }
        if (ret != nullptr) {
            if (desc.contains("main"))
                ret->setMainForm(true);
        }
    } else if (str == "Box") {
        ret = new BaseWidget(parent);
        ret->setWindowTitle(map["title"].toString());
        QString direction = desc["direction"].toString();
        QLayout *layout;
        if (direction == "H") {
            layout = new QHBoxLayout(ret);
            layout->setSpacing(0);
            layout->setContentsMargins(5, 5, 5, 5);
        } else {
            layout = new QVBoxLayout(ret);
            layout->setSpacing(0);
            layout->setContentsMargins(5, 5, 5, 5);
        }
        foreach (QVariant v, desc["childs"].toList()) {
            QVariantMap m = v.toMap();
            BaseWidget *w = privateParseWidgets(ref, m, settings, map, parent);
            if (w != nullptr) {
                layout->addWidget(w);
                QObject::connect(w, &BaseWidget::closeParent, ret, &BaseWidget::closeParent);
                QObject::connect(w, &BaseWidget::appendWidget, ret, &BaseWidget::appendWidget);
                QObject::connect(ret, &BaseWidget::idChanged, w, &BaseWidget::changeId);
                QObject::connect(w, &BaseWidget::idChanged, ret, &BaseWidget::idChanged);
            }
        }
    } else if (str == "Splitter") {
        ret = new BaseWidget(parent);
        ret->setWindowTitle(map["title"].toString());
        QString direction = desc["direction"].toString();
        QSplitter *layout = new QSplitter(ret);
        if (direction == "H") {
            // layout = new QHBoxLayout(ret);
            // layout->setSpacing(0);
            layout->setOrientation(Qt::Horizontal);
            layout->setContentsMargins(5, 5, 5, 5);
        } else {
            // layout = new QVBoxLayout(ret);
            // layout->setSpacing(0);
            layout->setOrientation(Qt::Vertical);
            layout->setContentsMargins(5, 5, 5, 5);
        }
        foreach (QVariant v, desc["childs"].toList()) {
            QVariantMap m = v.toMap();
            BaseWidget *w = privateParseWidgets(ref, m, settings, map, parent);
            if (w != nullptr) {
                layout->addWidget(w);
                QObject::connect(w, &BaseWidget::closeParent, ret, &BaseWidget::closeParent);
                QObject::connect(w, &BaseWidget::appendWidget, ret, &BaseWidget::appendWidget);
                QObject::connect(ret, &BaseWidget::idChanged, w, &BaseWidget::changeId);
                QObject::connect(w, &BaseWidget::idChanged, ret, &BaseWidget::idChanged);
            }
        }
    } else if (str == "Scroll") {
        ret = new BaseWidget(parent);
        ret->setWindowTitle(map["title"].toString());
        QLayout *layout;
        layout = new QHBoxLayout(ret);
        layout->setSpacing(0);
        layout->setContentsMargins(5, 5, 5, 5);
        QScrollArea *scroll = new QScrollArea(ret);
        layout->addWidget(scroll);
        QString direction = desc["direction"].toString();

        if (direction == "H") {
            scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        } else if (direction == "V") {
            scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        } else {
        }
        scroll->setWidgetResizable(true);
        foreach (QVariant v, desc["childs"].toList()) {
            QVariantMap m = v.toMap();
            BaseWidget *w = privateParseWidgets(ref, m, settings, map, parent);
            if (w != nullptr) {
                scroll->setWidget(w);
                QObject::connect(w, &BaseWidget::closeParent, ret, &BaseWidget::closeParent);
                QObject::connect(w, &BaseWidget::appendWidget, ret, &BaseWidget::appendWidget);
                QObject::connect(ret, &BaseWidget::idChanged, w, &BaseWidget::changeId);
                QObject::connect(w, &BaseWidget::idChanged, ret, &BaseWidget::idChanged);
                break;
            }
        }
    }

    return ret;
}

BaseWidget *Interface::createWidget(const QString &desc, const RestSettings *settings,
                                    const QVariantMap &map, QWidget *parent) {
    QVariantMap vals = QJsonDocument::fromJson(desc.toUtf8()).object().toVariantMap();
    QVariantMap ref;
    BaseWidget *ptr = privateParseWidgets(ref, vals, settings, map, parent);
    if (ptr != nullptr)
        ptr->setMainForm(true);

    return ptr;
}

void sekura_init_resources() { Q_INIT_RESOURCE(resources); }
