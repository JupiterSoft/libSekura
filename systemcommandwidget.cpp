/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "systemcommandwidget.h"
#include "sekura.h"
#include "ui_systemcommandwidget.h"
#include <QFileDialog>
#include <QJsonDocument>

using namespace Sekura;

SystemCommandWidget::SystemCommandWidget(const QString &sys, QWidget *parent)
    : QWidget(parent), ui(new Ui::SystemCommandWidget), m_systemKey(sys) {
    ui->setupUi(this);
    m_settings = new RestSettings;
    Interface::setSettings(m_settings);
    m_client = new RestClient;
    connect(m_client, &RestClient::success, this, &SystemCommandWidget::success);
    connect(m_client, &RestClient::error, this, &SystemCommandWidget::error);
    connect(ui->pbCommands, &QPushButton::clicked, this, &SystemCommandWidget::pbCommands);
    connect(ui->pbSend, &QPushButton::clicked, this, &SystemCommandWidget::pbSend);
    m_settings->setHeaderValue("Content-Type", "application/json");
    m_settings->setHeaderValue("charset", "utf-8");
}

SystemCommandWidget::~SystemCommandWidget() {
    delete ui;
    delete m_settings;
    delete m_client;
}

void SystemCommandWidget::success(const QJsonObject &obj) {
    qDebug() << obj;
    ui->te->setText(QJsonDocument(obj).toJson());
}

void SystemCommandWidget::error(const QJsonObject &obj) { qDebug() << obj; }

void SystemCommandWidget::pbCommands() {
    m_fileName = QFileDialog::getOpenFileName(this, "Open file", QString(), "*.json");
}

void SystemCommandWidget::pbSend() {
    m_settings->setPath(ui->leAddress->text() + "/sekura/api/v1.0");
    m_settings->setHeaderValue("sekura-application", m_systemKey.toUtf8());
    if (m_fileName.isEmpty())
        return;
    QFile file(m_fileName);
    if (!file.open(QIODevice::ReadOnly))
        return;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject())
        return;
    QJsonObject obj = doc.object();
    QVariantMap req = obj.toVariantMap();
    req["transaction"] = "cmd";
    m_client->request("/query", "POST", req);
}
