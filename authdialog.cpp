/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "authdialog.h"
#include "ui_authdialog.h"

#include "restclient.h"
#include "restsettings.h"

#include <QJsonObject>

using namespace Sekura;

AuthDialog::AuthDialog(RestSettings *settings, bool requestKeyButton, QWidget *parent)
    : QDialog(parent), ui(new Ui::AuthDialog), m_settings(settings) {
    ui->setupUi(this);
    m_client = new RestClient(settings);
    connect(m_client, &RestClient::success, this, &AuthDialog::success);
    connect(m_client, &RestClient::error, this, &AuthDialog::error);
    if (requestKeyButton) {
        ui->pbRequestKey->setVisible(true);
    } else {
        ui->pbRequestKey->setVisible(false);
    }
}

AuthDialog::~AuthDialog() {
    delete ui;
    delete m_client;
}

void AuthDialog::accept() {

    if (ui->leKey->text().isEmpty() || ui->lePin->text().isEmpty()) {
        ui->lError->setText(tr("Key or PIN is EMPTY!"));
        return;
    }

    QVariantMap map;
    map["key"] = ui->leKey->text();
    map["pin"] = ui->lePin->text();
    map["device"] = "test";
    m_client->request("/auth", "GET", map);

    /// TODO wait
    // todo start wait process
}

void AuthDialog::success(const QJsonObject &obj) {
    if (obj.contains("token")) {
        QString token = obj.value("token").toString();
        m_settings->setHeaderValue("Authorization", token.toUtf8());
        m_settings->save();
        QDialog::accept();
    } else if (obj.contains("newkey")) {
        QString newkey = obj.value("newkey").toString();
        ui->leKey->setText(newkey);
    } else {
        ui->lError->setText(tr("Unknown error"));
        ui->pbRequestKey->setEnabled(true);
    }
}

void AuthDialog::error(const QJsonObject &obj) {
    QString e = obj.value("error").toString();
    QString ref = obj.value("ref").toString();
    ui->lError->setText(ref);
}

void AuthDialog::on_pbRequestKey_clicked() {
    QVariantMap map;
    map["newkey"] = 1;
    m_client->request("/auth", "GET", map);
    ui->pbRequestKey->setEnabled(false);
}
