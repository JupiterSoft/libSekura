/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "wizarditemwidget.h"
#include "ui_wizarditemwidget.h"

#include <QJsonDocument>

using namespace Sekura;

WizardItemWidget::WizardItemWidget(const QString &form, QWidget *parent)
    : QDialog(parent), ui(new Ui::WizardItemWidget), m_form(form) {
    ui->setupUi(this);
    connect(ui->pbClose, &QPushButton::clicked, this, &WizardItemWidget::reject);
    connect(ui->pbSave, &QPushButton::clicked, this, &WizardItemWidget::pbSave_clicked);
    connect(ui->pbAdd, &QPushButton::clicked, this, &WizardItemWidget::pbAdd_clicked);
    connect(ui->pbDel, &QPushButton::clicked, this, &WizardItemWidget::pbDel_clicked);
    connect(ui->cmbItem, &QComboBox::currentIndexChanged, this,
            &WizardItemWidget::cmbItem_currentChanged);
    connect(ui->cmbModel, &QComboBox::currentIndexChanged, this,
            &WizardItemWidget::cmbModel_currentChanged);

    connect(ui->tree, &QTreeWidget::itemSelectionChanged, this,
            &WizardItemWidget::tree_itemSelectionChanged);
    connect(ui->tw, &QTableWidget::itemChanged, this, &WizardItemWidget::tw_itemChanged);

    m_client = new RestClient(Interface::settings(), this);
    connect(m_client, &RestClient::success, this, &WizardItemWidget::success);
    connect(m_client, &RestClient::error, this, &WizardItemWidget::error);

    QVariantMap map, q;
    map["transaction"] = "tables";
    q["name"] = "get_tables";
    q["table"] = "custom";
    q["query"] =
        "SELECT a.id AS tid, a.table_name AS tname, b.id AS fid, b.field_name AS fname "
        "FROM a_tables a, a_fields b WHERE a.id = b.table_id ORDER BY a.table_name, b.field_order";
    map["queries"] = QVariantList() << q;
    m_client->request("/query", "GET", map);

    parseJson();

    m_currentItem = nullptr;
    ui->gb->setEnabled(false);
    m_currentItem = nullptr;
}

WizardItemWidget::~WizardItemWidget() { delete ui; }

void WizardItemWidget::pbSave_clicked() {
    /// TODO make form
    if (m_currentItem != nullptr)
        saveItem(m_currentItem);
    toJson();
    QDialog::accept();
}

void WizardItemWidget::pbAdd_clicked() {

    QTreeWidgetItem *item = nullptr;
    if (m_currentItem != nullptr) {
        const QVariantMap &mp = m_items[m_currentItem];
        QString type = mp["type"].toString();
        if ((type == "Box") || (type == "Splitter") || (type == "Scroll") || (type == "Tab") ||
            (type == "Tool") || (type == "Spacer") || (type == "GroupBox")) {
            item = new QTreeWidgetItem(m_currentItem, QStringList() << "Box"
                                                                    << "");
        }
    } else {
        item = new QTreeWidgetItem(ui->tree, QStringList() << "Box"
                                                           << "");
    }
    if (item != nullptr) {
        item->setFlags(item->flags() | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
        QVariantMap map;
        map["type"] = "Box";
        map["direction"] = "H";
        m_items[item] = map;
    }
}

void WizardItemWidget::pbDel_clicked() {
    if (m_currentItem != nullptr) {
        removeFromItem(m_currentItem);
        delete m_currentItem;
        m_currentItem = nullptr;
        fillItem(m_currentItem);
    }
}

void WizardItemWidget::cmbItem_currentChanged(int a) {
    if (m_currentItem == nullptr)
        return;
    switch (a) {
    case 0: ///< Item
    case 1: ///< Table
    case 2: ///< Tree
    case 3: ///< List
    case 4: ///< TableEdit
        ui->sw->setCurrentIndex(0);
        break;
    case 5:  ///< Box
    case 6:  ///< Splitter
    case 7:  ///< Scroll
    case 8:  ///< Tab
    case 9:  ///< Tool
    case 10: ///< Spacer
    case 11: ///< GroupBox
        ui->sw->setCurrentIndex(1);
        break;
    case 12: ///< Command
        ui->sw->setCurrentIndex(2);
        break;
    case 13: ///< CustomModel
        ui->sw->setCurrentIndex(3);
        break;
    case 14: ///< Element
        ui->sw->setCurrentIndex(4);
        break;
    }
    m_currentItem->setText(0, ui->cmbItem->currentText());
}

void WizardItemWidget::cmbModel_currentChanged(int a) {
    if (m_currentItem == nullptr)
        return;
    m_currentItem->setText(1, ui->cmbModel->currentText());
    ui->tw->setRowCount(0);
    if (m_currentItem == nullptr)
        return;
    QVariantMap map = m_items[m_currentItem];
    QStringList hidden, filter, filter_column;
    QList<int> filter_tables, filter_column2;
    if (map.contains("filter"))
        filter = map["filter"].toStringList();
    if (map.contains("hidden"))
        hidden = map["hidden"].toStringList();
    QString code = m_idTables[a];

    foreach (QString f, filter) {
        QStringList lst = f.split("=");
        filter_column << lst[0].trimmed();
        QString tbl, col;
        if (lst[1] == "%id") {
            tbl = m_tables[a];
            col = "id";
        } else {
            QStringList lst2 = lst[1].split(".");
            tbl = lst2[0].trimmed();
            col = lst2[1].trimmed();
        }
        QString tbl_id;
        for (int i = 0; i < m_tables.size(); i++)
            if (m_tables[i] == tbl) {
                filter_tables << i;
                tbl_id = m_idTables[i];
                break;
            }
        for (int i = 0; i < m_fields[tbl_id].size(); i++)
            if (m_fields[tbl_id][i] == col) {
                filter_column2 << i;
                break;
            }
    }

    ui->tw->setRowCount(m_fields[code].size());
    int row = 0;
    foreach (QString str, m_fields[code]) {
        QTableWidgetItem *ptr = new QTableWidgetItem(str);
        ptr->setFlags(ptr->flags() & ~Qt::ItemIsEditable);
        QComboBox *box1 = new QComboBox;
        QComboBox *box2 = new QComboBox;

        box1->addItems(m_tables);

        connect(box1, &QComboBox::currentIndexChanged, this, [this, box2](int a) {
            QString c = m_idTables[a];
            box2->clear();
            box2->addItems(m_fields[c]);
        });

        int c = 0;
        if (filter_column.contains(str)) {
            int t = filter_column.indexOf(str);
            box1->setCurrentIndex(filter_tables[t]);
            box2->setCurrentIndex(filter_column2[t]);
        }

        QCheckBox *cbox = new QCheckBox;
        if (hidden.contains(str))
            cbox->setChecked(true);

        ui->tw->setItem(row, 0, ptr);
        ui->tw->setCellWidget(row, 1, box1);
        ui->tw->setCellWidget(row, 2, box2);
        ui->tw->setCellWidget(row, 3, cbox);

        row++;
    }
}

void WizardItemWidget::tree_itemSelectionChanged() {
    saveItem(m_currentItem);
    QList<QTreeWidgetItem *> sel = ui->tree->selectedItems();
    m_currentItem = nullptr;
    foreach (QTreeWidgetItem *i, sel) {
        m_currentItem = i;
        break;
    }
    fillItem(m_currentItem);
}

void WizardItemWidget::tw_itemChanged(QTableWidgetItem *item) {}

void WizardItemWidget::success(const QJsonObject &obj) {
    qDebug() << obj;
    QVariantMap response = obj.toVariantMap();
    QString t = response["transaction"].toString();
    if (t == "tables") {
        QVariantList data = response["response"].toMap()["get_tables"].toList();
        m_tables.clear();
        m_idTables.clear();
        m_fields.clear();
        m_idFields.clear();
        m_tables << "None";
        m_idTables << "-";
        m_fields["-"] << "None";
        m_idFields["-"] << "None";
        foreach (QVariant v, data) {
            QVariantMap m = v.toMap();
            QString tname = m["tname"].toString();
            QString tid = m["tid"].toString();
            QString fname = m["fname"].toString();
            QString fid = m["fid"].toString();
            if (!m_fields.contains(tid)) {
                m_tables << tname;
                m_idTables << tid;
            }
            m_fields[tid] << fname;
            m_idFields[tid] << fid;
        }
        ui->cmbModel->clear();
        ui->cmbModel->addItems(m_tables);
    }
}

void WizardItemWidget::error(const QJsonObject &obj) { qDebug() << obj; }

void WizardItemWidget::parseJson() {
    m_data = QJsonDocument::fromJson(m_form.toUtf8()).object().toVariantMap();
    if (m_data.contains("caption"))
        ui->leCaption->setText(m_data["caption"].toString());
    parse(m_data, nullptr);
}

void WizardItemWidget::toJson() {
    QTreeWidgetItem *item = ui->tree->invisibleRootItem();
    for (int i = 0; item->childCount(); i++) {
        m_data = mapItem(item->child(i));
        break;
        /// Берем только первый элемент
    }

    QJsonDocument doc(QJsonObject::fromVariantMap(m_data));
    m_form = doc.toJson(QJsonDocument::Compact);
}

void WizardItemWidget::parse(const QVariantMap &map, QTreeWidgetItem *parent) {
    QVariantMap m;
    for (QVariantMap::ConstIterator it = map.constBegin(); it != map.constEnd(); ++it) {
        if (it.key() == "childs") {
            continue;
        }
        m[it.key()] = *it;
    }
    QTreeWidgetItem *item = nullptr;
    QStringList list;
    list << m["type"].toString();
    if (m.contains("model"))
        list << m["model"].toString();
    else
        list << "";
    if (parent == nullptr) {
        item = new QTreeWidgetItem(ui->tree, list);
    } else {
        item = new QTreeWidgetItem(parent, list);
    }
    item->setFlags(item->flags() | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
    m_items[item] = m;

    if (map.contains("childs")) {
        foreach (QVariant v, map["childs"].toList()) {
            parse(v.toMap(), item);
        }
    }
}

void WizardItemWidget::saveItem(QTreeWidgetItem *item) {
    if (item != nullptr) {
        QVariantMap map;
        QString type = ui->cmbItem->currentText();
        map["type"] = type;
        if (!ui->leCaption->text().isEmpty())
            map["caption"] = ui->leCaption->text();
        QStringList lst;
        lst << type;
        if (ui->sw->currentIndex() == 0) {
            // Item | Table | Tree | TableEdit
            if (ui->cbMainForm->isChecked())
                map["main"] = 1;
            map["model"] = ui->cmbModel->currentText();
            lst << ui->cmbModel->currentText();
            QVariantList hidden;
            QVariantList filter;
            for (int i = 0; i < ui->tw->rowCount(); i++) {
                QString id = ui->tw->item(i, 0)->text();
                QCheckBox *box = qobject_cast<QCheckBox *>(ui->tw->cellWidget(i, 3));
                if (box->isChecked())
                    hidden << id;
                QComboBox *box1 = qobject_cast<QComboBox *>(ui->tw->cellWidget(i, 1));
                QComboBox *box2 = qobject_cast<QComboBox *>(ui->tw->cellWidget(i, 2));
                QString table = box1->currentText();
                QString column = box2->currentText();
                if ((table == ui->cmbModel->currentText()) && (column == "id")) {
                    filter << "id=%id";
                } else if (table != "None") {
                    filter << id + "=" + table + "." + column;
                }
            }
            if (!filter.empty())
                map["filter"] = filter;
            if (!hidden.empty())
                map["hidden"] = hidden;
            if (!ui->leID->text().isEmpty())
                map["id"] = ui->leID->text();
        } else if (ui->sw->currentIndex() == 1) {
            /// Layouts
            if (ui->rbH->isChecked())
                map["direction"] = "H";
            else if (ui->rbV->isChecked())
                map["direction"] = "V";
            else if (ui->rbHV->isChecked())
                map["direction"] = "HV";
            lst << "";
        } else if (ui->sw->currentIndex() == 2) {
            /// Command
            lst << "";
        } else if (ui->sw->currentIndex() == 3) {
            /// Custom Model
            lst << "";
        } else if (ui->sw->currentIndex() == 4) {
            /// Element of custom model
            lst << "";
        }
        m_items[item] = map;
        item->setText(0, lst[0]);
        item->setText(1, lst[1]);
    }
}

void WizardItemWidget::fillItem(QTreeWidgetItem *item) {
    if (item == nullptr) {
        ui->gb->setEnabled(false);
    } else {
        ui->gb->setEnabled(true);
        QVariantMap m = m_items[item];
        QString type = m["type"].toString();
        if (m.contains("caption"))
            ui->leCaption->setText(m["caption"].toString());
        else
            ui->leCaption->setText("");

        if (type == "Item")
            ui->cmbItem->setCurrentIndex(0);
        else if (type == "Table")
            ui->cmbItem->setCurrentIndex(1);
        else if (type == "Tree")
            ui->cmbItem->setCurrentIndex(2);
        else if (type == "List")
            ui->cmbItem->setCurrentIndex(3);
        else if (type == "TableEdit")
            ui->cmbItem->setCurrentIndex(4);
        else if (type == "Box")
            ui->cmbItem->setCurrentIndex(5);
        else if (type == "Splitter")
            ui->cmbItem->setCurrentIndex(6);
        else if (type == "Scroll")
            ui->cmbItem->setCurrentIndex(7);
        else if (type == "Tab")
            ui->cmbItem->setCurrentIndex(8);
        else if (type == "Tool")
            ui->cmbItem->setCurrentIndex(9);
        else if (type == "Spacer")
            ui->cmbItem->setCurrentIndex(10);
        else if (type == "GroupBox")
            ui->cmbItem->setCurrentIndex(11);
        else if (type == "Command")
            ui->cmbItem->setCurrentIndex(12);
        else if (type == "CustomModel")
            ui->cmbItem->setCurrentIndex(13);
        else if (type == "Element")
            ui->cmbItem->setCurrentIndex(14);

        if (m.contains("model")) {
            QString model = m["model"].toString();
            for (int i = 0; i < m_tables.size(); i++) {
                if (m_tables[i] == model) {
                    ui->cmbModel->setCurrentIndex(i);
                }
            }
        } else {
            ui->cmbModel->setCurrentIndex(0);
        }
        if (m.contains("direction")) {
            QString dir = m["direction"].toString();
            if (dir == "H") {
                ui->rbH->setChecked(true);
            } else if (dir == "V") {
                ui->rbV->setChecked(true);
            } else if (dir == "HV") {
                ui->rbHV->setChecked(true);
            }
        }
        if (m.contains("main")) {
            ui->cbMainForm->setChecked(true);
        } else {
            ui->cbMainForm->setChecked(false);
        }

        if (m.contains("id")) {
            ui->leID->setText(m["id"].toString());
        } else {
            ui->leID->setText("");
        }
    }
}

QVariantMap WizardItemWidget::mapItem(QTreeWidgetItem *item) {
    QVariantMap ret = m_items[item];
    QVariantList childs;
    for (int i = 0; i < item->childCount(); i++)
        childs << mapItem(item->child(i));
    if (!childs.empty())
        ret["childs"] = childs;
    return ret;
}

void WizardItemWidget::removeFromItem(QTreeWidgetItem *item) {
    for (int i = 0; i < item->childCount(); i++)
        removeFromItem(item->child(i));
    m_items.remove(item);
}
