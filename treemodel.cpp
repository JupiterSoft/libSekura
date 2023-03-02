/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "treemodel.h"
#include "modelfilter.h"
#include "restclient.h"

#include <QQueue>

using namespace Sekura;

/*!
 * \brief The TreeModel::TreeItem class - скрытый класс иерархических элементов дерева
 */
class TreeModel::TreeItem {
  public:
    /*!
     * \brief TreeItem - конструктор
     * \param data - список данных
     * \param parentItem - родительский элемент
     */
    explicit TreeItem(const QVariantList &data, TreeItem *parentItem = nullptr) {
        m_itemData = data;
        m_parentItem = parentItem;
    }
    ~TreeItem() { qDeleteAll(m_childItems); }

    /*!
     * \brief removeChilds - удалить все подчиненные элементы
     */
    void removeChilds() {
        foreach (TreeItem *item, m_childItems)
            delete item;
        m_childItems.clear();
    }

    /*!
     * \brief appendChild - добавить подчиненный элемент
     * \param child - элемент
     */
    void appendChild(TreeItem *child) { m_childItems.append(child); }

    /*!
     * \brief updateData - обновить данные
     * \param data - данные
     */
    void updateData(const QVariantList &data) { m_itemData = data; }

    /*!
     * \brief child - получить подчиненный по номеру строки
     * \param row - номер строки
     * \return указатель на подчиенный объект
     */
    TreeItem *child(int row) {
        if (row < 0 || row >= m_childItems.size())
            return nullptr;
        return m_childItems.at(row);
    }

    /*!
     * \brief childCount возвращает количество подчиненных объектов
     * \return количество подчиненных
     */
    int childCount() const { return m_childItems.count(); }

    /*!
     * \brief columnCount получить количество колонок
     * \return количество колонок
     */
    int columnCount() const { return m_itemData.count(); }
    QVariant data(int column) const {
        if (column < 0 || column >= m_itemData.size())
            return QVariant();
        return m_itemData.at(column);
    }

    /*!
     * \brief row возвращает количество строк
     * \return количество строк
     */
    int row() const {
        if (m_parentItem)
            return m_parentItem->m_childItems.indexOf(const_cast<TreeItem *>(this));

        return 0;
    }

    /*!
     * \brief parentItem - родительский элемент
     * \return указатель на родительский элемент
     */
    TreeItem *parentItem() { return m_parentItem; }

    /*!
     * \brief setId - установка идентификатора
     * \param str - идентификатор
     */
    void setId(const QString &str) { m_id = str; }

    /*!
     * \brief id - получить идентификатор
     * \return значение идентификатора
     */
    const QString &id() const { return m_id; }

    QVariantMap values(const QVariantList &indx) const {
        QVariantMap ret;
        for (int i = 0; i < indx.size(); i++) {
            ret[indx[i].toString()] = m_itemData[i];
        }
        ret["id"] = m_id;
        return ret;
    }

    /*!
     * \brief findId - найти подчиненный элемент по идентификатору
     * \param str - идентификатор
     * \return указатель на подиченный элемент или nullptr
     */
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

    /*!
     * \brief fillParents - заполнение по идентификаторам установка указателей
     * \param p
     */
    void fillParents(QMap<QString, TreeItem *> &p) {
        foreach (TreeItem *ptr, m_childItems) {
            p[ptr->id()] = ptr;
            ptr->fillParents(p);
        }
    }

    /*!
     * \brief makeValues - по установка заполнить рекусивно элементы
     * \param map - установки
     * \param headers - заголовки
     * \param vals - список значений заголовков
     */
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

/*!
 * \brief TreeModel::TreeModel - конструктор
 * \param model - модель/таблица
 * \param settings - настройки подключение
 * \param parent - родительский объект
 */

TreeModel::TreeModel(ModelFilter *filter, QObject *parent) : QAbstractItemModel{parent} {
    m_root = nullptr;
    m_modelFilter = filter;
    m_model = m_modelFilter->value("temp", "model").toString();
    m_only_my = false;
    if (m_modelFilter->contains("temp", "onlyMy"))
        m_only_my = true;
    connect(m_modelFilter, &ModelFilter::itemChanged, this, &TreeModel::filterChanged);
    /// TODO вместо строки использовать VariantMap передать параметр только мои
    m_client = new RestClient(Interface::settings(), this);
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

/*!
 * \brief TreeModel::data получить данные
 * \param index - индекс
 * \param role - роль отображения
 * \return значение по индексу и роли
 */
QVariant TreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem *>(index.internalPointer());

    return item->data(index.column());
}

/*!
 * \brief TreeModel::headerData - получить значения заголовка
 * \param section - секция
 * \param orientation - направление
 * \param role - роль отображение
 * \return значение заголовка
 */
QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (m_root != nullptr && orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_root->data(section);

    return QVariant();
}

/*!
 * \brief TreeModel::index - построить индекс по строке и колонке
 * \param row - строка
 * \param column - колонка
 * \param parent - родительский индекс
 * \return индекс
 */
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

/*!
 * \brief TreeModel::parent - получить родителя индекса
 * \param index родитель
 * \return индекс родителя
 */
QModelIndex TreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem *>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if ((parentItem == m_root) || (parentItem == nullptr))
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

/*!
 * \brief TreeModel::rowCount - количство строк в данном элементе
 * \param parent - элемент
 * \return количество строк у элемента
 */
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

/*!
 * \brief TreeModel::columnCount - количество колонок
 * \param parent - не используется
 * \return возвращает количество колонок
 */
int TreeModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid())
        return static_cast<TreeItem *>(parent.internalPointer())->columnCount();
    if (m_root == nullptr)
        return 0;
    return m_root->columnCount();
}

/*!
 * \brief TreeModel::stretchField - поле по которому растягивается дерево отображения
 * \return номер колонки
 */
int TreeModel::stretchField() const {
    int ret = 0;
    foreach (QVariant str, m_view) {
        if (str.toString() == m_stretch)
            return ret;
        ret++;
    }
    return -1;
}

/*!
 * \brief TreeModel::code - получить идентификатор для элемента
 * \param index - индекс
 * \return код элемента по индексу
 */
QString TreeModel::code(const QModelIndex &index) {
    if (!index.isValid())
        return "";

    TreeItem *item = static_cast<TreeItem *>(index.internalPointer());
    QVariantMap m = item->values(m_view);
    m_modelFilter->setValue(m_model, m);

    return item->id();
}

/*!
 * \brief TreeModel::remove - удалить элемент из базы данных по индексу
 * \param index - индекс
 */
void TreeModel::remove(const QModelIndex &index) {
    QVariantMap req, q;
    q["table"] = m_model;
    q["name"] = m_model;
    q["id"] = code(index);
    req["queries"] = QVariant(QVariantList() << q);
    req["transaction"] = "delete";
    m_client->request("/query", "DELETE", req);
}

/*!
 * \brief TreeModel::makeValues - заполнить значения
 * \param map - карта значений
 * \param header - заголовки
 */
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

/*!
 * \brief TreeModel::reload - перечитать данные из СУБД
 */
void TreeModel::reload() {
    QVariantMap req, q;

    q["table"] = m_model;
    q["name"] = m_model;
    q["fields"] = m_fields;
    m_filter.clear();
    for (QVariantMap::ConstIterator it = m_filterFromDesc.constBegin();
         it != m_filterFromDesc.constEnd(); ++it) {
        const QVariantMap valr = m_modelFilter->value(it.key());
        const QVariantMap temp = it->toMap();
        m_filter[temp["mcol"].toString()] = valr[temp["rcol"].toString()].toString();
    }
    /// TODO передать параметр только мои
    if (!m_filter.isEmpty()) {
        /// TODO set filter
        QString str;
        bool first = true;
        for (QVariantMap::Iterator it = m_filter.begin(); it != m_filter.end(); ++it) {
            if (first)
                first = false;
            else
                str += " AND ";
            str += "a." + it.key() + " = :" + it.key();
        }
        q["filter"] = str;
        /// TODO set variables
        q["variables"] = m_filter;
    }
    req["queries"] = QVariant(QVariantList() << q);
    req["transaction"] = "refresh";
    if (m_only_my)
        req["onlyMy"] = true;
    m_client->request("/query", "GET", req);
}

/*!
 * \brief TreeModel::changeIndex - обработка изменения индекса для таблицы
 * \param table - таблица
 * \param id - значение индекса
 */
void TreeModel::changeIndex(const QString &table, const QString &id) {
    if (m_fk.contains(table)) {
        m_filter[m_fk[table]] = id;
        reload();
    }
}

QList<int> TreeModel::headerIndex(const QVariantList &lst) {
    QList<int> ret;
    foreach (QVariant v, lst) {
        QString id = v.toString();
        int i = 0;
        bool finded = false;
        foreach (QVariant mv, m_view) {
            if (mv.toString() == id) {
                finded = true;
                break;
            }
            i++;
        }
        if (finded) {
            ret << i;
        } else {
            ret << -1;
        }
    }
    return ret;
}

/*!
 * \brief TreeModel::setFilter - установка фильтра
 * \param filter - фильтр
 */
// void TreeModel::setFilter(const QVariantMap &filter) {
//     for (QVariantMap::ConstIterator it = filter.constBegin(); it != filter.constEnd(); ++it) {
//         m_filter[it.key()] = *it;
//     }
//     reload();
// }

/*!
 * \brief TreeModel::removeFromFilter - удалить значение из фильтра
 * \param key ключ
 */
// void TreeModel::removeFromFilter(const QString &key) {
//     m_filter.remove(key);
//     reload();
// }

void TreeModel::filterChanged(const QString &index, const QVariantMap &value) {
    /// TODO вставить обработку
    if (m_filterFromDesc.contains(index))
        reload();
}

/*!
 * \brief TreeModel::success - обработка при успешном ответе сервера
 * \param obj - JSON объект ответ сервера
 */
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
        if (bottomRight.isValid()) {
            emit dataChanged(topLeft, bottomRight);
            emit layoutChanged();
        }
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

/*!
 * \brief TreeModel::error - обработка ошибки на сервере
 * \param obj
 */
void TreeModel::error(const QJsonObject &obj) { qDebug() << obj; }
