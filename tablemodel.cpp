/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "tablemodel.h"
#include "restclient.h"
#include "restsettings.h"

using namespace Sekura;

TableModel::TableModel(const QString &model, RestSettings *settings, QObject *parent)
    : QAbstractTableModel{parent} {
    m_model = model;
    m_client = new RestClient(settings, this);
    connect(m_client, &RestClient::success, this, &TableModel::success);
    connect(m_client, &RestClient::error, this, &TableModel::error);
    m_viewCode = false;
    QVariantMap map;
    map["transaction"] = "model";
    map["model"] = m_model;
    map["type"] = "table";
    m_client->request("/model", "GET", map);
}

TableModel::~TableModel() { delete m_client; }

int TableModel::rowCount(const QModelIndex &parent) const { return m_data.size(); }

int TableModel::columnCount(const QModelIndex &parent) const { return m_headers.size(); }

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

QString TableModel::code(const QModelIndex &index) const {
    int row = index.row();
    if (row < m_codes.size())
        return m_codes[row];
    return "";
}

void TableModel::reload() {
    QVariantMap req, q;

    q["table"] = m_model;
    q["name"] = m_model;
    q["fields"] = m_fields;
    /// TODO set filter
    /// TODO set variables
    req["queries"] = QVariant(QVariantList() << q);
    req["transaction"] = "refresh";
    m_client->request("/query", "GET", req);
}

void TableModel::remove(const QModelIndex &index) {}

void TableModel::setFilter(const QVariantMap &filter) {
    m_filter = filter;
    reload();
}

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
        reload();
        emit headerDataChanged(Qt::Orientation::Horizontal, 0, columnCount() - 1);
    }
}

void TableModel::error(const QJsonObject &obj) { qDebug() << obj; }
