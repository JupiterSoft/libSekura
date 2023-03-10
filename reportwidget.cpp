/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "reportwidget.h"
#include "modelfilter.h"
#include "restclient.h"
#include "ui_reportwidget.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QTemporaryFile>

using namespace Sekura;

ReportWidget::ReportWidget(ModelFilter *filter, QWidget *parent)
    : BaseWidget(filter, parent), ui(new Ui::ReportWidget) {
    ui->setupUi(this);
    m_report = m_modelFilter->value("temp", "report").toMap();
    m_client = new RestClient;
    connect(m_client, &RestClient::success, this, &ReportWidget::success);
    connect(m_client, &RestClient::error, this, &ReportWidget::error);

    m_view = new QWebEngineView(this);
    ui->reportWidgetLayout->addWidget(m_view);

    // resize(800, 600);

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
    // QJsonDocument doc(obj);
    QVariantMap response = obj.toVariantMap();
    QString t = response["transaction"].toString();
    if (t == "refresh") {
        // QString file_name1 = QDir::tempPath() + "/" + Interface::genUFID() + ".js";
        // QString file_name1 = QDir::tempPath() + "/data.js";
        // QString file_name1 = "data.js";
        // qDebug() << file_name1;
        QFile file0(":/html/report.html");
        file0.open(QIODevice::ReadOnly);
        QString html = file0.readAll();
        html = html.replace("$caption$", "TEST");
        // html = html.replace("$temp_file$", file_name1);
        //  html = html.replace("$colCaption$", u8R"([{title:'ID'},{title:'Name'}])");
        QString temp = "var dataSet = [";
        // QFile file1(file_name1);
        // file1.open(QIODevice::WriteOnly);
        QVariantList resp = response["response"].toList();
        // file1.write("var dataSet = [");

        bool first = true;
        foreach (QVariant r, resp) {
            QVariantMap v = r.toMap();
            QString t = ",";
            if (first) {
                first = false;
                t = "";
            }
            QString x = QString(t + "[\"" + v["id"].toString() + "\", \"" +
                                v["table_name"].toString() + "\"]");
            // file1.write(x.toUtf8());
            temp += x;
        }

        // file1.write("];");
        temp += "];";
        // file1.write(u8R"($(document).ready(function () {
        //     $('#example').DataTable({data: dataSet,columns: [{title:'ID'},{title:'Name'}]});
        // });)");
        temp += u8R"($(document).ready(function () {
            $('#example').DataTable({data: dataSet,columns: [{title:'ID'},{title:'Name'}]});
        });)";

        //  file1.close();
        html = html.replace("$temp$", temp);
        // qDebug() << html;
        m_view->setHtml(html);
    }
    // ui->textEdit->setText(doc.toJson(QJsonDocument::Indented));
}

void ReportWidget::error(const QJsonObject &obj) { qDebug() << obj; }
