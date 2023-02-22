/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "itemwidget.h"
#include "restclient.h"
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
    } else if (t == "model") {
        // Построить модель
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
