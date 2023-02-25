/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "tablemodel.h"
#include "restclient.h"
#include "restsettings.h"

using namespace Sekura;

/*!
 * \brief TableModel::TableModel - конструктор
 * \param model - имя модели/таблица
 * \param settings - настройки подключения
 * \param parent - родительский объект
 */
TableModel::TableModel(const QString &model, const RestSettings *settings, QObject *parent)
    : QAbstractTableModel{parent} {
    m_model = model;
    m_client = new RestClient(settings, this);
    connect(m_client, &RestClient::success, this, &TableModel::success);
    connect(m_client, &RestClient::error, this, &TableModel::error);
    m_viewCode = false;
    m_initialized = false;
    QVariantMap map;
    map["transaction"] = "model";
    map["model"] = m_model;
    map["type"] = "table";
    m_client->request("/model", "GET", map);
}

/*!
 * \brief TableModel::TableModel - конструктор с фильтром
 * \param model - имя модели/таблицы
 * \param settings - настройки подключения
 * \param filter - фильтр
 * \param parent - родительский объект
 */
TableModel::TableModel(const QString &model, const RestSettings *settings,
                       const QVariantMap &filter, QObject *parent)
    : QAbstractTableModel{parent} {
    m_model = model;
    m_filter = filter;
    m_client = new RestClient(settings, this);
    connect(m_client, &RestClient::success, this, &TableModel::success);
    connect(m_client, &RestClient::error, this, &TableModel::error);
    m_viewCode = false;
    m_initialized = false;
    QVariantMap map;
    map["transaction"] = "model";
    map["model"] = m_model;
    map["type"] = "table";
    m_client->request("/model", "GET", map);
}

TableModel::~TableModel() { delete m_client; }

/*!
 * \brief TableModel::rowCount - количество строк
 * \param parent - не используется
 * \return возвращает количество строк в модели
 */
int TableModel::rowCount(const QModelIndex &parent) const { return m_data.size(); }

/*!
 * \brief TableModel::columnCount - количество колонок
 * \param parent - не используется
 * \return возвращает количество колонок в модели
 */
int TableModel::columnCount(const QModelIndex &parent) const { return m_headers.size(); }

/*!
 * \brief TableModel::data - получить данные для индекса
 * \param index - индекс
 * \param role - роль отображения
 * \return возвращает данные по индексу для роли отборажения
 */
QVariant TableModel::data(const QModelIndex &index, int role) const {
    int row = index.row();
    int col = index.column();
    QString ind = m_view_data[col];
    if (row < m_data.size()) {
        const QVariant &val = m_data[row][ind];
        switch (role) {
        case Qt::DisplayRole:
            if (val.userType() == QMetaType::Bool)
                return QString("");
            return val;
        case Qt::FontRole:
            /// TODO insert factor to bold
            //     QFont boldFont;
            //     boldFont.setBold(true);
            //     return boldFont;
            break;
        case Qt::BackgroundRole:
            /// TODO insert factor to color
            // if (row == 1 && col == 2) // change background only for cell(1,2)
            //     return QBrush(Qt::red);
            break;
        case Qt::TextAlignmentRole:
            /// TODO insert factor to text align
            // if (row == 0 && col == 1) // change text alignment only for cell(1,1)
            //     return int(Qt::AlignHCenter | Qt::AlignVCenter);
            break;
        case Qt::CheckStateRole:
            if (val.userType() == QMetaType::Bool) {
                if (val.toBool())
                    return Qt::Checked;
                else
                    return Qt::Unchecked;
            }
            break;
        }
    }
    return QVariant();
}

/*!
 * \brief TableModel::headerData - функция для получения имени заголовка
 * \param section - номер
 * \param orientation - горизонтальное или вертикальное
 * \param role - роль отображение
 * \return значение заголовка для секции горизонтальной или вертикальной
 */
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        if (section < m_headers.size())
            return m_headers[section];
    }
    if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
        if (m_viewCode) {
            if (section < m_codes.size())
                return m_codes[section];
        } else
            return section + 1;
    }
    return QVariant();
}

/*!
 * \brief TableModel::stretchField - возвращает номер поля которое должно растягиваться
 * \return номер поля
 */
int TableModel::stretchField() const {
    int ret = 0;
    foreach (QString str, m_view_data) {
        if (str == m_stretch)
            return ret;
        ret++;
    }
    return -1;
}

/*!
 * \brief TableModel::code - получение кода для индекса
 * \param index - индекс
 * \return код
 */
QString TableModel::code(const QModelIndex &index) const {
    int row = index.row();
    if (row < m_codes.size())
        return m_codes[row];
    return "";
}

/*!
 * \brief TableModel::value - получение значение для индекса
 * \param index - индекс
 * \return код
 */
QString TableModel::value(const QModelIndex &index) const {
    int row = index.row();
    if (row < m_codes.size())
        return m_data[row][m_stretch].toString();
    return "";
}

/*!
 * \brief TableModel::reload - перечитывает данные из СУБД
 */
void TableModel::reload() {
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

/*!
 * \brief TableModel::remove - удаляет данные из таблицы СУБД
 * \param index выделенный индекс
 */
void TableModel::remove(const QModelIndex &index) {
    QVariantMap req, q;
    q["table"] = m_model;
    q["name"] = m_model;
    q["id"] = code(index);
    req["queries"] = QVariant(QVariantList() << q);
    req["transaction"] = "delete";
    m_client->request("/query", "DELETE", req);
}

/*!
 * \brief TableModel::changeIndex - сообщает что для таблицы table изменился текущий индекс
 * в случае если данная таблица связана с table происходит перечитывание
 * \param table - таблица
 * \param id - индекс
 */
void TableModel::changeIndex(const QString &table, const QString &id) {
    if (m_fk.contains(table)) {
        m_filter[m_fk[table]] = id;
        reload();
    }
}

/*!
 * \brief TableModel::setFilter - установка постоянного фильтра
 * \param filter - фильтр
 */
void TableModel::setFilter(const QVariantMap &filter) {
    for (QVariantMap::ConstIterator it = filter.constBegin(); it != filter.constEnd(); ++it) {
        m_filter[it.key()] = *it;
    }
    reload();
}

/*!
 * \brief TableModel::removeFromFilter - удалить значение из фильтра
 * \param key - ключ
 */
void TableModel::removeFromFilter(const QString &key) {
    m_filter.remove(key);
    reload();
}

/*!
 * \brief TableModel::success - обработка успешного ответа от сервера
 * \param obj - JSON объект ответ сервера
 */
void TableModel::success(const QJsonObject &obj) {
    qDebug() << obj;
    QVariantMap response = obj.toVariantMap();
    QString t = response["transaction"].toString();
    if (t == "refresh") {
        QVariantList data = response["response"].toMap()[m_model].toList();
        m_data.clear();
        m_codes.clear();
        foreach (QVariant d, data) {
            QVariantMap mp = d.toMap();
            m_data.append(mp);
            m_codes.append(mp["id"].toString());
        }
        QModelIndex topLeft = index(0, 0);
        QModelIndex bottomRight = index(rowCount() - 1, columnCount() - 1);

        emit dataChanged(topLeft, bottomRight);
        emit layoutChanged();
    } else if (t == "model") {
        // reload();
        QVariantMap data = response["response"].toMap();
        m_fields = data["fields"].toList();

        m_headers.clear();
        QVariantList lst = data["header"].toList();
        foreach (QVariant h, lst)
            m_headers.append(h.toString());

        m_view_data.clear();
        lst = data["view"].toList();
        foreach (QVariant v, lst)
            m_view_data.append(v.toString());

        QVariantMap fk = data["foreign_keys"].toMap();
        for (QVariantMap::Iterator it = fk.begin(); it != fk.end(); ++it)
            m_fk[it.key()] = it->toString();

        reload();
        m_stretch = data["stretch"].toString();
        m_buttons = data["buttons"].toList();
        m_form_edit = data["form_edit"].toString();
        emit initialized();
        emit headerDataChanged(Qt::Orientation::Horizontal, 0, columnCount() - 1);
    } else if (t == "delete") {
        reload();
    }
}

/*!
 * \brief TableModel::error - обработка ошибки
 * \param obj
 */
void TableModel::error(const QJsonObject &obj) { qDebug() << obj; }
