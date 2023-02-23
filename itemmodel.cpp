/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "itemmodel.h"
#include "baseitem.h"
#include "combobox.h"

using namespace Sekura;

ItemModel::ItemModel(const QVariantMap &map, const RestSettings *settings, QObject *parent)
    : QObject{parent} {
    m_data = map;
    m_client = new RestClient(settings, this);
    connect(m_client, &RestClient::success, this, &ItemModel::success);
    connect(m_client, &RestClient::error, this, &ItemModel::error);
    QVariantMap req;
    m_data["table"] = m_data["model"];
    req["transaction"] = "model";
    req["model"] = m_data["model"];
    req["type"] = "item";
    if (m_data["filter"].toMap().contains("id")) {
        m_isNew = false;
    } else {
        m_isNew = true;
    }
    m_client->request("/model", "GET", req);
}

ItemModel::~ItemModel() { delete m_client; }

void ItemModel::save() {
    if (m_isNew) {
        // создать post обращение
        QVariantMap req, q, f;
        q["table"] = m_data["table"];
        q["name"] = m_data["table"];

        for (QMap<QString, BaseItem *>::Iterator it = m_items.begin(); it != m_items.end(); ++it) {
            if (!m_blockAlways[it.key()]) {
                f[it.key()] = (*it)->value();
            }
        }

        q["fields"] = f;
        req["queries"] = QVariantList() << q;
        req["transaction"] = "insert";
        m_client->request("/query", "POST", req);
        emit setEnabled(false);
    } else {
        // проверить все что изменились и вставить в patch
        QVariantMap req, q, f;
        q["table"] = m_data["table"];
        q["name"] = m_data["table"];

        bool changed = false;

        for (QMap<QString, BaseItem *>::Iterator it = m_items.begin(); it != m_items.end(); ++it) {
            if (m_blockOnEdit[it.key()] || m_blockAlways[it.key()])
                continue;
            if ((*it)->value() != m_values[it.key()]) {
                f[it.key()] = (*it)->value();
                changed = true;
            }
        }
        f["id"] = m_data["filter"].toMap()["id"];
        if (changed) {
            q["fields"] = QVariant(f);
            req["queries"] = QVariant(QVariantList() << q);
            req["transaction"] = "update";
            m_client->request("/query", "PATCH", req);
            emit setEnabled(false);
        }
    }
}

void ItemModel::reload() {
    QVariantMap req, q;
    if (m_isNew) {
        req["queries"] = QVariant(QVariantList() << m_queries);
    } else {
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
                str += "a." + it.key() + " = :" + it.key();
            }
            q["filter"] = str;
            /// TODO set variables
            q["variables"] = filter;
        }
        req["queries"] = QVariant(QVariantList() << m_queries << q);
    }
    req["transaction"] = "refresh";
    m_client->request("/query", "GET", req);
}

void ItemModel::setItem(const QString &index, BaseItem *ptr) {
    if (ptr != nullptr) {
        m_items[index] = ptr;
        ptr->setCaption(m_captions[index]);
        if (m_blockAlways[index])
            ptr->setEnabled(false);
        if (!m_isNew && m_blockOnEdit[index])
            ptr->setEnabled(false);
    }
}

void ItemModel::success(const QJsonObject &obj) {
    qDebug() << obj;
    QVariantMap response = obj.toVariantMap();
    QString t = response["transaction"].toString();
    if (t == "refresh") {
        // Получить данные
        QVariantMap tbls = response["response"].toMap();
        for (QVariantMap::Iterator it = tbls.begin(); it != tbls.end(); ++it) {
            QVariantList lst = it->toList();
            if (it.key() == m_data["table"]) {
                foreach (QVariant v, lst) {
                    QVariantMap m = v.toMap();
                    for (QVariantMap::ConstIterator it = m.constBegin(); it != m.constEnd(); ++it) {
                        if (m_items.contains(it.key())) {
                            m_items[it.key()]->setValue(*it);
                            m_values[it.key()] = *it;
                        }
                    }
                }
            } else {
                if (m_items.contains(it.key())) {
                    ComboBox *b = dynamic_cast<ComboBox *>(m_items[it.key()]);
                    if (b != nullptr)
                        b->setModel(lst);
                }
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
            m_blockOnEdit[id] = m["block_on_edit"].toBool();
            m_blockAlways[id] = m["block_always"].toBool();
            if (m.contains("fk_table")) {
                m_refs[id] = true;
            }
            m_captions[id] = m["caption"].toString();
        }
        emit connectInterface(resp["fields"]);
        m_data["fields"] = mf;
        m_queries = resp["queries"].toList();

        reload();
    } else if (t == "insert") {
        emit setEnabled(true);
        foreach (QVariant v, response["response"].toMap()[m_data["table"].toString()].toList()) {
            QVariantMap map = v.toMap();
            m_items["created_at"]->setValue(map["created_at"]);
            m_items["id"]->setValue(map["id"]);
            m_data["filter"].toMap()["id"] = map["id"];
            m_isNew = false;
            break;
        }
        for (QMap<QString, bool>::Iterator it = m_blockOnEdit.begin(); it != m_blockOnEdit.end();
             ++it)
            if (*it) {
                m_items[it.key()]->setEnabled(false);
            }
        emit parentReload();
    } else if (t == "update") {
        emit setEnabled(true);
        emit parentReload();
    }
}

void ItemModel::error(const QJsonObject &obj) { qDebug() << obj; }
