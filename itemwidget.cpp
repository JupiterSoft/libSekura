/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "itemwidget.h"
#include "checkbox.h"
#include "combobox.h"
#include "datetimeedit.h"
#include "lineedit.h"
#include "restclient.h"
#include "spinbox.h"
#include "ui_itemwidget.h"

using namespace Sekura;

ItemWidget::ItemWidget(const QVariantMap &map, const RestSettings *settings, QWidget *parent)
    : QWidget(parent), ui(new Ui::ItemWidget) {
    ui->setupUi(this);
    m_data = map;
    m_client = new RestClient(settings, this);
    connect(m_client, &RestClient::success, this, &ItemWidget::success);
    connect(m_client, &RestClient::error, this, &ItemWidget::error);
    QVariantMap req;
    m_data["table"] = m_data["model"];
    req["transaction"] = "model";
    req["model"] = m_data["model"];
    req["type"] = "item";
    m_client->request("/model", "GET", req);
}

ItemWidget::~ItemWidget() {
    delete ui;
    delete m_client;
}

void ItemWidget::success(const QJsonObject &obj) {
    qDebug() << obj;
    QVariantMap response = obj.toVariantMap();
    QString t = response["transaction"].toString();
    if (t == "refresh") {
        // Получить данные
        QVariantList lst = response["response"].toMap()[m_data["table"].toString()].toList();
        foreach (QVariant v, lst) {
            QVariantMap m = v.toMap();
            for (QVariantMap::ConstIterator it = m.constBegin(); it != m.constEnd(); ++it) {
                if (m_items.contains(it.key()))
                    m_items[it.key()]->setValue(*it);
            }
        }
    } else if (t == "model") {
        QVariantMap resp = response["response"].toMap();
        // Построить модель
        QVariantList fields = resp["fields"].toList();
        QVariantList mf;
        foreach (QVariant v, fields) {
            QVariantMap m = v.toMap();
            QString id = m["name"].toString();
            mf << id;
            BaseItem *ptr = nullptr;
            QString type = m["ui_desk"].toString();
            if (m.contains("fk_table")) {
                m_refs[id] = true;
            }
            if (type.left(8) == "LineEdit") {
                ptr = new LineEdit(0, this); ///<<вставить инициализацию
            } else if (type.left(12) == "DateTimeEdit") {
                ptr = new DateTimeEdit(0, this);
            } else if (type.left(8) == "ComboBox") {
                ptr = new ComboBox(0, this);
            } else if (type.left(8) == "CheckBox") {
                ptr = new CheckBox(0, this);
            } else if (type.left(7) == "SpinBox") {
                ptr = new SpinBox(0, this);
            }
            if (ptr != nullptr) {
                ptr->setCaption(m["caption"].toString());
                ui->baseLayout->addWidget(ptr);
                m_items[id] = ptr;
            }
        }
        m_data["fields"] = mf;
        reload();
    }
}

void ItemWidget::error(const QJsonObject &obj) { qDebug() << obj; }

void ItemWidget::reload() {
    QVariantMap req, q;

    q["table"] = m_data["table"];
    q["name"] = m_data["model"];
    q["fields"] = m_data["fields"];
    if (m_data.contains("filter") && !m_data["filter"].toMap().isEmpty()) {
        /// TODO set filter
        QString str;
        QVariantMap filter = m_data["filter"].toMap();
        bool first = true;
        for (QVariantMap::Iterator it = filter.begin(); it != filter.end(); ++it) {
            if (first)
                first = false;
            else
                str += " AND ";
            str += it.key() + " = :" + it.key();
        }
        q["filter"] = str;
        /// TODO set variables
        q["variables"] = filter;
    }
    req["queries"] = QVariant(QVariantList() << q);
    req["transaction"] = "refresh";
    m_client->request("/query", "GET", req);
}
