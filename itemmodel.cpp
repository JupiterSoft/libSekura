/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "itemmodel.h"
#include "baseitem.h"
#include "combobox.h"
#include "modelfilter.h"

using namespace Sekura;

/*!
 * \brief ItemModel::ItemModel - конструктор
 * \param map - установки модели
 * \param settings - настройки подключения
 * \param parent - родитель
 */
ItemModel::ItemModel(ModelFilter *filter, QObject *parent) : QObject{parent} {
    m_modelFilter = filter;
    m_model = m_modelFilter->value("temp", "model").toString();
    m_client = new RestClient(Interface::settings(), this);
    connect(m_client, &RestClient::success, this, &ItemModel::success);
    connect(m_client, &RestClient::error, this, &ItemModel::error);
    QVariantMap req;
    // m_data["table"] = m_data["model"];
    // m_model = m_data["table"].toString();
    // m_filter = m_data["filter"].toMap();
    req["transaction"] = "model";
    req["model"] = m_model;
    req["type"] = "item";
    if (m_modelFilter->value(m_model).contains("isNew")) {
        m_isNew = true;
    } else {
        m_isNew = false;
    }
    m_client->request("/model", "GET", req);
}

ItemModel::~ItemModel() { delete m_client; }

/*!
 * \brief ItemModel::save - сохранить модель, если это новый то POST запрос, если существующий то
 * PATCH запрос
 */
void ItemModel::save() {
    if (m_isNew) {
        // создать post обращение
        QVariantMap req, q, f;
        q["table"] = m_model;
        q["name"] = m_model;

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
        q["table"] = m_model;
        q["name"] = m_model;

        bool changed = false;

        for (QMap<QString, BaseItem *>::Iterator it = m_items.begin(); it != m_items.end(); ++it) {
            if (m_blockOnEdit[it.key()] || m_blockAlways[it.key()])
                continue;
            if ((*it)->value() != m_values[it.key()]) {
                f[it.key()] = (*it)->value();
                changed = true;
            }
        }
        // f["id"] = m_items["id"]->value(); //< может отстутствовать!!!
        f["id"] = m_modelFilter->value(m_model, "id");
        if (changed) {
            q["fields"] = QVariant(f);
            req["queries"] = QVariant(QVariantList() << q);
            req["transaction"] = "update";
            m_client->request("/query", "PATCH", req);
            emit setEnabled(false);
        }
    }
}

/*!
 * \brief ItemModel::reload - обновить информацию об элементе
 */
void ItemModel::reload() {
    QVariantMap req, q;
    if (m_isNew) {
        req["queries"] = QVariant(QVariantList() << m_queries);
    } else {
        q["table"] = m_model;
        q["name"] = m_model;
        q["fields"] = m_fields;
        /// TODO переделать фильтрацию
        q["filter"] = "a.id = :id";
        QVariantMap vars;
        vars["id"] = m_modelFilter->value(m_model, "id");
        q["variables"] = vars;
        req["queries"] = QVariant(QVariantList() << m_queries << q);
    }
    req["transaction"] = "refresh";
    m_client->request("/query", "GET", req);
}

/*!
 * \brief ItemModel::setItem - устанавливает элемент управления для поля
 * \param index - имя поля
 * \param ptr - элемент управления
 */
void ItemModel::setItem(const QString &index, BaseItem *ptr) {
    if (ptr != nullptr) {
        m_items[index] = ptr;
        ptr->setCaption(m_captions[index]);
        if (m_blockAlways[index])
            ptr->setEnabled(false); ///< TODO read only
        if (!m_isNew && m_blockOnEdit[index])
            ptr->setEnabled(false); ///< TODO read only
    }
}

/*!
 * \brief ItemModel::setNew - взводит флаг \b{Это Новый}
 */
void ItemModel::setNew() {
    m_isNew = true;
    /// TODO Добавить очистку данных
}

/*!
 * \brief ItemModel::createTable - создает таблицу в базе данных
 */
void ItemModel::createTable() {
    if (m_values.contains("table_name")) {
        QVariantMap req;
        req["transaction"] = "create table";
        req["model"] = m_values["table_name"];
        m_client->request("/model", "POST", req);
    }
}

/*!
 * \brief ItemModel::dropTable - удаляет таблицу из базы данных
 */
void ItemModel::dropTable() {
    if (m_values.contains("table_name")) {
        QVariantMap req;
        req["transaction"] = "drop table";
        req["model"] = m_values["table_name"];
        m_client->request("/model", "DELETE", req);
    }
}

/*!
 * \brief ItemModel::setFilter - устанавливает новый фильтр и перечитывает элемент
 * \param filter
 */
// void ItemModel::setFilter(const QVariantMap &filter) {
//     for (QVariantMap::ConstIterator it = filter.constBegin(); it != filter.constEnd(); ++it) {
//         m_filter[it.key()] = *it;
//     }
//     reload();
// }

/*!
 * \brief ItemModel::removeFromFilter - удалить из фильтра и перечитать элемент
 * \param key
 */
// void ItemModel::removeFromFilter(const QString &key) {
//     m_filter.remove(key);
//     reload();
// }

/*!
 * \brief ItemModel::success - ответ от сервер без ошибок
 * \param obj - JSON объект
 */
void ItemModel::success(const QJsonObject &obj) {
    qDebug() << obj;
    QVariantMap response = obj.toVariantMap();
    QString t = response["transaction"].toString();
    if (t == "refresh") {
        // Получить данные
        QVariantMap tbls = response["response"].toMap();
        for (QVariantMap::Iterator it = tbls.begin(); it != tbls.end(); ++it) {
            QVariantList lst = it->toList();
            if (it.key() == m_model) {
                foreach (QVariant v, lst) {
                    QVariantMap m = v.toMap();
                    for (QVariantMap::ConstIterator jt = m.constBegin(); jt != m.constEnd(); ++jt) {
                        if (m_items.contains(jt.key())) {
                            m_items[jt.key()]->setValue(*jt);
                            m_values[jt.key()] = *jt;
                        } else if (jt.key().right(4) == "_ref") {
                            QString k = jt.key();
                            k = k.remove("_ref");
                            if (m_items.contains(k)) {
                                m_items[k]->setViewValue(*jt);
                            }
                        }
                    }
                }
            } else {
                if (m_items.contains(it.key())) {
                    ComboBox *b = dynamic_cast<ComboBox *>(m_items[it.key()]);
                    if (b != nullptr) {
                        b->setModel(lst);
                        // if(m_filter.contains())
                    }
                }
            }
        }
    } else if (t == "model") {
        QVariantMap resp = response["response"].toMap();
        // Построить модель
        QVariantList fields = resp["fields"].toList();
        // QVariantMap filter = resp["child_filter"].toMap();
        m_fields.clear();
        foreach (QVariant v, fields) {
            QVariantMap m = v.toMap();
            QString id = m["name"].toString();
            m_fields << id;
            BaseItem *ptr = nullptr;
            m_blockOnEdit[id] = m["block_on_edit"].toBool();
            m_blockAlways[id] = m["block_always"].toBool();
            if (m.contains("fk_table")) {
                m_refs[id] = true;
                //                if (filter.contains(m["fk_table"].toString())) {
                //                    const QVariantMap &f1 =
                //                    filter[m["fk_table"].toString()].toMap(); m_filter[id] =
                //                    f1["id"].toString();
                //                }
            }
            m_captions[id] = m["caption"].toString();
        }
        emit connectInterface(resp["fields"]);
        // m_data["fields"] = mf;
        m_queries = resp["queries"].toList();
        // emit initialized();
        reload();
    } else if (t == "insert") {
        emit setEnabled(true);
        foreach (QVariant v, response["response"].toMap()[m_model].toList()) {
            QVariantMap map = v.toMap();
            m_items["created_at"]->setValue(map["created_at"]);
            m_items["id"]->setValue(map["id"]);
            m_modelFilter->setValue(m_model, "id", map["id"]);
            // m_data["filter"].toMap()["id"] = map["id"];
            m_isNew = false;
            // emit idChanged(m_model, map["id"].toString());
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
    } else if (t == "create table") {

    } else if (t == "drop table") {
    }
}

/*!
 * \brief ItemModel::error - ошибка от сервера
 * \param obj
 */
void ItemModel::error(const QJsonObject &obj) { qDebug() << obj; }
