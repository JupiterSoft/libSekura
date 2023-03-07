/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "reportwidget.h"
#include "modelfilter.h"
#include "restclient.h"
#include "ui_reportwidget.h"

#include <QJsonDocument>

using namespace Sekura;

ReportWidget::ReportWidget(ModelFilter *filter, QWidget *parent)
    : BaseWidget(filter, parent), ui(new Ui::ReportWidget) {
    ui->setupUi(this);
    m_report = m_modelFilter->value("temp", "report").toMap();
    m_client = new RestClient;
    connect(m_client, &RestClient::success, this, &ReportWidget::success);
    connect(m_client, &RestClient::error, this, &ReportWidget::error);
    reload();
}

ReportWidget::~ReportWidget() { delete ui; }

void ReportWidget::reload() {
    QVariantMap req = m_report;
    req["transaction"] = "refresh";
    m_client->request("/report", "GET", req);
}

void ReportWidget::success(const QJsonObject &obj) {
    qDebug() << obj;
    QJsonDocument doc(obj);
    ui->textEdit->setText(doc.toJson(QJsonDocument::Indented));
    /// TODO
}

void ReportWidget::error(const QJsonObject &obj) { qDebug() << obj; }
