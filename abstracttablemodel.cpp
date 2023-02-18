/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "abstracttablemodel.h"
#include "restclient.h"
#include "restsettings.h"

#include <QBrush>
#include <QFont>

using namespace Sekura;

AbstractTableModel::AbstractTableModel(RestSettings *settings, QObject *parent)
    : QAbstractTableModel{parent} {
    m_client = new RestClient(settings, this);
    connect(m_client, &RestClient::success, this, &AbstractTableModel::success);
    connect(m_client, &RestClient::error, this, &AbstractTableModel::error);
    m_viewCode = false;
}

AbstractTableModel::~AbstractTableModel() { delete m_client; }

int AbstractTableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_data.size();
}

int AbstractTableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent)
    return m_headers.size();
}

QVariant AbstractTableModel::data(const QModelIndex &index, int role) const {
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

QVariant AbstractTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
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

QString AbstractTableModel::code(const QModelIndex &index) const {
    int row = index.row();
    if (row < m_codes.size())
        return m_codes[row];
    return "";
}
