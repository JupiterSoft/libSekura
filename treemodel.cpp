/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "treemodel.h"
#include "restclient.h"

#include <QQueue>

using namespace Sekura;

class TreeModel::TreeItem {
  public:
    explicit TreeItem(const QVariantList &data, TreeItem *parentItem = nullptr) {
        m_itemData = data;
        m_parentItem = parentItem;
    }
    ~TreeItem() { qDeleteAll(m_childItems); }

    void removeChilds() {
        foreach (TreeItem *item, m_childItems)
            delete item;
        m_childItems.clear();
    }

    void appendChild(TreeItem *child) { m_childItems.append(child); }

    void updateData(const QVariantList &data) { m_itemData = data; }

    TreeItem *child(int row) {
        if (row < 0 || row >= m_childItems.size())
            return nullptr;
        return m_childItems.at(row);
    }
    int childCount() const { return m_childItems.count(); }
    int columnCount() const { return m_itemData.count(); }
    QVariant data(int column) const {
        if (column < 0 || column >= m_itemData.size())
            return QVariant();
        return m_itemData.at(column);
    }
    int row() const {
        if (m_parentItem)
            return m_parentItem->m_childItems.indexOf(const_cast<TreeItem *>(this));

        return 0;
    }
    TreeItem *parentItem() { return m_parentItem; }
    void setId(const QString &str) { m_id = str; }
    const QString &id() const { return m_id; }

    TreeItem *findId(const QString &str) {
        if (str == m_id)
            return this;
        foreach (TreeItem *ptr, m_childItems) {
            TreeItem *ret = ptr->findId(str);
            if (ret != nullptr)
                return ret;
        }
        return nullptr;
    }

    void fillParents(QMap<QString, TreeItem *> &p) {
        foreach (TreeItem *ptr, m_childItems) {
            p[ptr->id()] = ptr;
            ptr->fillParents(p);
        }
    }

    void makeValues(QVariantMap &map, const QStringList &headers, const QList<int> &vals) {
        for (int i = 0; i < headers.size(); i++) {
            map[headers[i]] = m_itemData[vals[i]];
        }
        QVariantList childs;
        foreach (TreeItem *ptr, m_childItems) {
            QVariantMap m;
            ptr->makeValues(m, headers, vals);
            childs << m;
        }
        map["childs"] = childs;
    }

  private:
    QList<TreeItem *> m_childItems;
    QVariantList m_itemData;
    TreeModel::TreeItem *m_parentItem;
    QString m_id;
};

TreeModel::TreeModel(const QString &model, const RestSettings *settings, QObject *parent)
    : QAbstractItemModel{parent} {
    m_root = nullptr;
    m_model = model;
    m_client = new RestClient(settings, this);
    connect(m_client, &RestClient::success, this, &TreeModel::success);
    connect(m_client, &RestClient::error, this, &TreeModel::error);
    m_initialized = false;
    QVariantMap map;
    map["transaction"] = "model";
    map["model"] = m_model;
    map["type"] = "tree";
    m_client->request("/model", "GET", map);
}

TreeModel::~TreeModel() {
    if (m_root != nullptr)
        delete m_root;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem *>(index.internalPointer());

    return item->data(index.column());
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (m_root != nullptr && orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_root->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<TreeItem *>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem *>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if ((parentItem == m_root) || (parentItem == nullptr))
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const {
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_root;
    else
        parentItem = static_cast<TreeItem *>(parent.internalPointer());
    if (parentItem == nullptr)
        return 0;
    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return static_cast<TreeItem *>(parent.internalPointer())->columnCount();
    if (m_root == nullptr)
        return 0;
    return m_root->columnCount();
}

int TreeModel::stretchField() const {
    int ret = 0;
    foreach (QVariant str, m_view) {
        if (str.toString() == m_stretch)
            return ret;
        ret++;
    }
    return -1;
}

QString TreeModel::code(const QModelIndex &index) {
    if (!index.isValid())
        return "";

    TreeItem *item = static_cast<TreeItem *>(index.internalPointer());

    return item->id();
}

void TreeModel::remove(const QModelIndex &index) {
    QVariantMap req, q;
    q["table"] = m_model;
    q["name"] = m_model;
    q["id"] = code(index);
    req["queries"] = QVariant(QVariantList() << q);
    req["transaction"] = "delete";
    m_client->request("/query", "DELETE", req);
}

void TreeModel::makeValues(QVariantMap &map, const QStringList &header) {
    QList<int> indexes;
    foreach (QString t, header) {
        for (int i = 0; i < m_headers.size(); i++) {
            if (m_headers[i].toString() == t) {
                indexes << i;
                break;
            }
        }
    }
    m_root->makeValues(map, header, indexes);
}

void TreeModel::reload() {
    QVariantMap req, q;

    q["table"] = m_model;
    q["name"] = m_model;
    q["fields"] = m_fields;
    if (!m_filter.isEmpty()) {
        /// TODO set filter
        QString str;
        bool first = true;
        for (QVariantMap::Iterator it = m_filter.begin(); it != m_filter.end(); ++it) {
            if (first)
                first = false;
            else
                str += " AND ";
            str += it.key() + " = :" + it.key();
        }
        q["filter"] = str;
        /// TODO set variables
        q["variables"] = m_filter;
    }
    req["queries"] = QVariant(QVariantList() << q);
    req["transaction"] = "refresh";
    m_client->request("/query", "GET", req);
}

void TreeModel::changeIndex(const QString &table, const QString &id) {
    if (m_fk.contains(table)) {
        m_filter[m_fk[table]] = id;
        reload();
    }
}

void TreeModel::setFilter(const QVariantMap &filter) {
    for (QVariantMap::ConstIterator it = filter.constBegin(); it != filter.constEnd(); ++it) {
        m_filter[it.key()] = *it;
    }
    reload();
}

void TreeModel::removeFromFilter(const QString &key) {
    m_filter.remove(key);
    reload();
}

void TreeModel::success(const QJsonObject &obj) {
    qDebug() << obj;
    QVariantMap response = obj.toVariantMap();
    QString t = response["transaction"].toString();
    if (t == "refresh") {
        QVariantList data = response["response"].toMap()[m_model].toList();
        QMap<QString, TreeItem *> parents;
        beginResetModel();
        m_root->removeChilds();
        parents[""] = m_root;
        QQueue<QVariantMap> queue;
        foreach (QVariant d, data) {
            QVariantMap mp = d.toMap();
            QString id = mp["id"].toString();
            QString parent = mp["parent_id"].toString();
            if (parents.contains(parent)) {
                QVariantList vals;
                foreach (QVariant v, m_view) {
                    vals.append(mp[v.toString()]);
                }
                TreeItem *item = new TreeItem(vals, parents[parent]);
                item->setId(id);
                parents[parent]->appendChild(item);
                parents[id] = item;
            } else {
                queue.enqueue(mp);
            }
        }
        while (!queue.isEmpty()) {
            QVariantMap mp = queue.dequeue();
            QString id = mp["id"].toString();
            QString parent = mp["parent_id"].toString();
            if (parents.contains(parent)) {
                QVariantList vals;
                foreach (QVariant v, m_view) {
                    vals.append(mp[v.toString()]);
                }
                TreeItem *item = new TreeItem(vals, parents[parent]);
                item->setId(id);
                parents[parent]->appendChild(item);
                parents[id] = item;
            } else {
                queue.enqueue(mp);
            }
        }
        endResetModel();

        QModelIndex topLeft = index(0, 0);
        QModelIndex bottomRight = index(rowCount() - 1, columnCount() - 1);
        emit dataChanged(topLeft, bottomRight);
        emit layoutChanged();
    } else if (t == "model") {
        // reload();
        QVariantMap data = response["response"].toMap();
        m_fields = data["fields"].toList();

        m_headers = data["header"].toList();
        m_view = data["view"].toList();

        m_stretch = data["stretch"].toString();
        m_buttons = data["buttons"].toList();
        m_form_edit = data["form_edit"].toString();

        QVariantMap fk = data["foreign_keys"].toMap();
        for (QVariantMap::Iterator it = fk.begin(); it != fk.end(); ++it)
            m_fk[it.key()] = it->toString();

        m_initialized = true;
        emit initialized();
        m_root = new TreeItem(m_headers);
        emit headerDataChanged(Qt::Orientation::Horizontal, 0, columnCount() - 1);
        reload();
    } else if (t == "delete") {
        reload();
    }
}

void TreeModel::error(const QJsonObject &obj) { qDebug() << obj; }
