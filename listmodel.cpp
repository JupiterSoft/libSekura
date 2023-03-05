/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "listmodel.h"

using namespace Sekura;

ListModel::ListModel(ModelFilter *filter, QObject *parent)
    : QObject{parent}, m_modelFilter(filter) {
    m_client = new RestClient(Interface::settings(), this);
    connect(m_client, &RestClient::success, this, &ListModel::success);
    connect(m_client, &RestClient::error, this, &ListModel::error);
    m_model = m_modelFilter->value("temp", "model").toString();
    QVariantMap map;
    map["transaction"] = "model";
    map["model"] = m_model;
    map["type"] = "list";
    m_client->request("/model", "GET", map);
}

ListModel::~ListModel() { delete m_client; }

QStringList ListModel::list() const {
    QStringList ret;
    foreach (QVariant v, m_data) {
        QVariantMap m = v.toMap();
        for (QVariantMap::ConstIterator it = m.constBegin(); it != m.constEnd(); ++it) {
            if (it.key() == "id")
                continue;
            ret << it->toString();
            break;
        }
    }
    return ret;
}

QVariantMap ListModel::data(int a) const { return m_data[a].toMap(); }

void ListModel::success(const QJsonObject &obj) {
    qDebug() << obj;
    QVariantMap response = obj.toVariantMap();
    QString t = response["transaction"].toString();
    if (t == "refresh") {
        QVariantList data = response["response"].toMap()[m_model].toList();
        m_data.clear();
        foreach (QVariant d, data) {
            QVariantMap mp = d.toMap();
            m_data.append(mp);
        }
        emit initialized();
    } else if (t == "model") {
        // reload();
        QVariantMap data = response["response"].toMap();
        m_fields = data["fields"].toList();
        QVariantMap req, q;
        q["table"] = m_model;
        q["name"] = m_model;
        q["fields"] = m_fields;
        req["queries"] = QVariant(QVariantList() << q);
        req["transaction"] = "refresh";
        m_client->request("/query", "GET", req);
    }
}

void ListModel::error(const QJsonObject &obj) { qDebug() << obj; }
