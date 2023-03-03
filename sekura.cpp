/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "sekura.h"
#include "checkbox.h"
#include "combobox.h"
#include "datatype.h"
#include "datetimeedit.h"
#include "foreignkey.h"
#include "itemwidget.h"
#include "lineedit.h"
#include "menu.h"
#include "modelfilter.h"
#include "spinbox.h"
#include "tablewidget.h"
#include "treewidget.h"

#include <QCryptographicHash>
#include <QDateTime>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QScrollArea>
#include <QSpacerItem>
#include <QSplitter>
#include <QTabWidget>
#include <QTimer>
#include <QToolBox>
#include <QUuid>

using namespace Sekura;

RestSettings *Interface::m_settings = nullptr;

/*!
 * \brief Sekura::Interface::genUFID - генерирует новую метку
 * \return метка
 */
QString Sekura::Interface::genUFID() {
    return QCryptographicHash::hash(QUuid::createUuid().toString().toUtf8(),
                                    QCryptographicHash::Sha256)
        .toBase64(QByteArray::Base64UrlEncoding);
}

/*!
 * \brief Sekura::Interface::genKey - генерирует новый ключ
 * \return ключ
 */
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

/*!
 * \brief Interface::createItem - создает новый элемент управления
 * \param m - описание элемента
 * \param parent - родитель
 * \return возвращает новый элемент управления
 */
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
    } else if (type.left(8) == "DataType") {
        ptr = new DataType(0, parent);
    } else if (type.left(10) == "ForeignKey") {
        int t = 0;
        QString str = type.split(";")[0].remove("ForeignKey").remove("(").remove(")");
        bool ok;
        t = str.toInt(&ok);
        if (!ok)
            t = 0;
        ForeignKey *le = new ForeignKey(t, parent); ///<<вставить инициализацию
        ptr = le;
    }
    return ptr;
}

/*!
 * \brief Interface::createMenu создает новое меню на основе описания
 * \param mb - описание меню
 * \param settings - настройки подключения к серверу
 * \param parent - родитеь
 * \return - новое меню
 */
Menu *Interface::createMenu(QMenuBar *mb, QObject *parent) {
    static Menu *menu = nullptr;
    if (menu == nullptr)
        menu = new Menu(mb, parent);
    return menu;
}

/*!
 * \brief privateParseWidgets - рекурсивная функция для разбора создания формы
 * \param ref - описание всех таблиц что есть на форме, для связывания
 * \param desc - описание формы
 * \param settings - настройки подключения
 * \param map - параметры
 * \param parent - родитель
 * \return созданный элемент управления
 */
BaseWidget *privateParseWidgets(ModelFilter *mfilter, const QVariantMap &desc, QWidget *parent) {
    BaseWidget *ret = nullptr;
    QString str = desc["type"].toString();
    bool mf_created = false;
    if (mfilter == nullptr) {
        mfilter = new ModelFilter;
        mf_created = true;
    }
    if ((str == "Table") || (str == "Tree") || (str == "Item")) {
        /// установить настройки в фильтр
        mfilter->remove("temp");
        mfilter->setValue("temp", "model", desc["model"]);
        mfilter->setValue("temp", "caption", desc["caption"]);
        if (desc.contains("filter")) {
            mfilter->setValue("temp", "filter", desc["filter"]);
        }
        if (desc.contains("hidden")) {
            mfilter->setValue("temp", "hidden", desc["hidden"]);
        }

        /// создать необходимый widget
        if (str == "Table") {
            ret = new TableWidget(mfilter, parent);
        } else if (str == "Tree") {
            ret = new TreeWidget(mfilter, parent);
        } else if (str == "Item") {
            /// не фильтруется
            ret = new ItemWidget(mfilter, parent);
        }
        if (ret != nullptr) {
            if (desc.contains("main"))
                ret->setMainForm(true);
        }
    } else {
        ret = new BaseWidget(mfilter, parent);
        if (mfilter->contains("temp", "caption"))
            ret->setWindowTitle(mfilter->value("temp", "caption").toString());

        if (str == "Box") {
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
                BaseWidget *w = privateParseWidgets(mfilter, m, parent);
                if (w != nullptr) {
                    layout->addWidget(w);
                    QObject::connect(w, &BaseWidget::closeParent, ret, &BaseWidget::closeParent);
                    QObject::connect(w, &BaseWidget::appendWidget, ret, &BaseWidget::appendWidget);
                    QObject::connect(w, &BaseWidget::parentReload, ret, &BaseWidget::parentReload);
                    // QObject::connect(ret, &BaseWidget::idChanged, w, &BaseWidget::changeId);
                    // QObject::connect(w, &BaseWidget::idChanged, ret, &BaseWidget::idChanged);
                }
            }
        } else if (str == "Splitter") {
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
                BaseWidget *w = privateParseWidgets(mfilter, m, parent);
                if (w != nullptr) {
                    layout->addWidget(w);
                    QObject::connect(w, &BaseWidget::closeParent, ret, &BaseWidget::closeParent);
                    QObject::connect(w, &BaseWidget::appendWidget, ret, &BaseWidget::appendWidget);
                    QObject::connect(w, &BaseWidget::parentReload, ret, &BaseWidget::parentReload);
                    // QObject::connect(ret, &BaseWidget::idChanged, w, &BaseWidget::changeId);
                    // QObject::connect(w, &BaseWidget::idChanged, ret, &BaseWidget::idChanged);
                }
            }
        } else if (str == "Scroll") {
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
                BaseWidget *w = privateParseWidgets(mfilter, m, parent);
                if (w != nullptr) {
                    scroll->setWidget(w);
                    QObject::connect(w, &BaseWidget::closeParent, ret, &BaseWidget::closeParent);
                    QObject::connect(w, &BaseWidget::appendWidget, ret, &BaseWidget::appendWidget);
                    QObject::connect(w, &BaseWidget::parentReload, ret, &BaseWidget::parentReload);
                    // QObject::connect(ret, &BaseWidget::idChanged, w, &BaseWidget::changeId);
                    // QObject::connect(w, &BaseWidget::idChanged, ret, &BaseWidget::idChanged);
                    break;
                }
            }
        } else if (str == "Tab") {
            QLayout *layout;
            layout = new QHBoxLayout(ret);
            layout->setSpacing(0);
            layout->setContentsMargins(5, 5, 5, 5);
            QTabWidget *tab = new QTabWidget(ret);
            layout->addWidget(tab);
            foreach (QVariant v, desc["childs"].toList()) {
                QVariantMap m = v.toMap();
                BaseWidget *w = privateParseWidgets(mfilter, m, parent);
                if (w != nullptr) {
                    tab->addTab(w, m["caption"].toString());
                    QObject::connect(w, &BaseWidget::closeParent, ret, &BaseWidget::closeParent);
                    QObject::connect(w, &BaseWidget::appendWidget, ret, &BaseWidget::appendWidget);
                    QObject::connect(w, &BaseWidget::parentReload, ret, &BaseWidget::parentReload);
                }
            }
        } else if (str == "Tool") {
            QLayout *layout;
            layout = new QHBoxLayout(ret);
            layout->setSpacing(0);
            layout->setContentsMargins(5, 5, 5, 5);
            QToolBox *tool = new QToolBox(ret);
            layout->addWidget(tool);
            foreach (QVariant v, desc["childs"].toList()) {
                QVariantMap m = v.toMap();
                BaseWidget *w = privateParseWidgets(mfilter, m, parent);
                if (w != nullptr) {
                    tool->addItem(w, m["caption"].toString());
                    QObject::connect(w, &BaseWidget::closeParent, ret, &BaseWidget::closeParent);
                    QObject::connect(w, &BaseWidget::appendWidget, ret, &BaseWidget::appendWidget);
                    QObject::connect(w, &BaseWidget::parentReload, ret, &BaseWidget::parentReload);
                }
            }
        } else if (str == "Spacer") {
            QLayout *layout;
            layout = new QHBoxLayout(ret);
            layout->setSpacing(0);
            layout->setContentsMargins(5, 5, 5, 5);

            QSpacerItem *tool;
            QString direction = desc["direction"].toString();
            if (direction == "H") {
                tool = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
            } else {
                tool = new QSpacerItem(10, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);
            }

            layout->addItem(tool);
        } else if (str == "GroupBox") {
            QString caption = desc["caption"].toString();
            QGroupBox *box = new QGroupBox(ret);
            QLayout *layout = new QHBoxLayout(ret);
            layout->setSpacing(0);
            layout->setContentsMargins(5, 5, 5, 5);
            layout->addWidget(box);
            box->setTitle(caption);
            layout = new QHBoxLayout(box);
            layout->setSpacing(0);
            layout->setContentsMargins(5, 5, 5, 5);
            foreach (QVariant v, desc["childs"].toList()) {
                QVariantMap m = v.toMap();
                BaseWidget *w = privateParseWidgets(mfilter, m, parent);
                if (w != nullptr) {
                    layout->addWidget(w);
                    QObject::connect(w, &BaseWidget::closeParent, ret, &BaseWidget::closeParent);
                    QObject::connect(w, &BaseWidget::appendWidget, ret, &BaseWidget::appendWidget);
                    QObject::connect(w, &BaseWidget::parentReload, ret, &BaseWidget::parentReload);
                    // QObject::connect(ret, &BaseWidget::idChanged, w, &BaseWidget::changeId);
                    // QObject::connect(w, &BaseWidget::idChanged, ret, &BaseWidget::idChanged);
                }
            }
        }
    }

    if (mf_created) {
        if (ret != nullptr)
            ret->setFilter(mfilter);
        else if (mfilter->ref() <= 0)
            delete mfilter;
    }

    return ret;
}

/*!
 * \brief Interface::createWidget - создать форму на основе описания
 * \param desc - описание формы
 * \param settings - настройки подключения
 * \param map - установки
 * \param parent - родительская форма
 * \return новая форма
 */
BaseWidget *Interface::createWidget(ModelFilter *ref, const QString &desc, QWidget *parent) {
    QVariantMap vals = QJsonDocument::fromJson(desc.toUtf8()).object().toVariantMap();
    BaseWidget *ptr = privateParseWidgets(ref, vals, parent);
    if (ptr != nullptr)
        ptr->setMainForm(true);

    return ptr;
}

void Interface::setSettings(RestSettings *settings) { m_settings = settings; }

RestSettings *Interface::settings() {
    Q_ASSERT(m_settings);
    return m_settings;
}

void sekura_init_resources() { Q_INIT_RESOURCE(resources); }
