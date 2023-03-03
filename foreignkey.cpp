/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "foreignkey.h"
#include "ui_foreignkey.h"

using namespace Sekura;

ForeignKey::ForeignKey(int type, QWidget *parent) : BaseItem(type, parent), ui(new Ui::ForeignKey) {
    ui->setupUi(this);
    m_client = nullptr;
    m_initialized = false;
    if (m_type == 0) {
        /// Главный элемент
        QWidget::setObjectName("ForeignKey_local_main");
        ui->lineEdit->setHidden(true);
        connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &ForeignKey::cmb_indexChanged);
        m_client = new RestClient(Interface::settings(), this);
        connect(m_client, &RestClient::success, this, &ForeignKey::success);
        connect(m_client, &RestClient::error, this, &ForeignKey::error);
        QVariantMap map, q;
        map["transaction"] = "tables";
        q["name"] = "get_tables";
        q["table"] = "custom";
        q["query"] = "SELECT a.table_name, b.field_name, a.field_view "
                     "FROM a_tables a, a_fields b "
                     "WHERE a.id = b.table_id AND b.field_type = \'UUID\' "
                     "ORDER BY a.table_name, b.field_order;";
        map["queries"] = QVariantList() << q;
        m_client->request("/query", "GET", map);
    } else if (m_type == 1) {
        ui->comboBox->setHidden(true);
        ForeignKey *fk = parent->findChild<ForeignKey *>("ForeignKey_local_main");
        connect(fk, &ForeignKey::valueTable, this, &ForeignKey::setValueFromParent);
    } else if (m_type == 2) {
        ui->comboBox->setHidden(true);
        ForeignKey *fk = parent->findChild<ForeignKey *>("ForeignKey_local_main");
        connect(fk, &ForeignKey::valueField, this, &ForeignKey::setValueFromParent);
    } else if (m_type == 3) {
        ui->lineEdit->setHidden(true);
        QStringList lst;
        lst << ""
            << "list"
            << "table";
        ui->comboBox->addItems(lst);
    }
}

ForeignKey::~ForeignKey() {
    delete ui;
    if (m_client != nullptr)
        delete m_client;
}

void ForeignKey::setCaption(const QString &str) { ui->label->setText(str); }

void ForeignKey::setValue(const QVariant &val) {
    m_value = val.toString();
    if (m_type == 0) {
        if (m_initialized) {
            for (int i = 0; i < m_list.size(); i++)
                if (m_value == m_list[i]) {
                    ui->comboBox->setCurrentIndex(i);
                    cmb_indexChanged(i);
                    break;
                }
        }
    } else if ((m_type == 1) || (m_type == 2)) {
        ui->lineEdit->setText(m_value);
    } else if (m_type == 3) {
        if (m_value == "list")
            ui->comboBox->setCurrentIndex(1);
        else if (m_value == "table")
            ui->comboBox->setCurrentIndex(2);
    }
}

QVariant ForeignKey::value() const {
    if ((m_type == 0) || (m_type == 3)) {
        return ui->comboBox->currentText();
    }
    return ui->lineEdit->text();
}

void ForeignKey::setViewValue(const QVariant &) {}

void ForeignKey::setReadOnly() {}

void ForeignKey::setValueFromParent(const QString &v) { ui->lineEdit->setText(v); }

void ForeignKey::success(const QJsonObject &obj) {
    qDebug() << obj;
    QVariantMap response = obj.toVariantMap();
    QString t = response["transaction"].toString();
    if (t == "tables") {
        QVariantList data = response["response"].toMap()["get_tables"].toList();
        int temp = 0, i = 1;
        m_list << "";
        m_tables << "";
        m_fields << "";
        foreach (QVariant v, data) {
            QVariantMap m = v.toMap();
            QString tname = m["table_name"].toString();
            QString tview = m["field_view"].toString();
            QString fname = m["field_name"].toString();
            QString index = tname + "(" + fname + ")";
            m_list << index;
            m_tables << tname;
            m_fields << tview;
            if (m_value == index) {
                temp = i;
            }
            i++;
        }
        ui->comboBox->clear();
        ui->comboBox->addItems(m_list);
        ui->comboBox->setCurrentIndex(temp);
        cmb_indexChanged(temp);
    }
}

void ForeignKey::error(const QJsonObject &obj) { qDebug() << obj; }

void ForeignKey::cmb_indexChanged(int a) {
    emit valueTable(m_tables[a]);
    emit valueField(m_fields[a]);
}
