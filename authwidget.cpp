/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "authwidget.h"
#include "authdialog.h"
#include "ui_authwidget.h"

#include <QSettings>

using namespace Sekura;

AuthWidget::AuthWidget(QWidget *parent) : QWidget(parent), ui(new Ui::AuthWidget) {
    ui->setupUi(this);
    m_settings = Interface::settings();
    m_settings->setHeaderValue("Content-Type", "application/json");
    m_settings->setHeaderValue("charset", "utf-8");

    connect(ui->comboBox, &QComboBox::currentIndexChanged, this, &AuthWidget::cmb_index);
    connect(ui->pbSave, &QPushButton::clicked, this, &AuthWidget::pb_save);
    connect(ui->pbDel, &QPushButton::clicked, this, &AuthWidget::pb_del);
    connect(ui->pbAuth, &QPushButton::clicked, this, &AuthWidget::pb_auth);

    reload();
}

AuthWidget::~AuthWidget() { delete ui; }

void AuthWidget::cmb_index(int a) {
    m_settings->load(ui->comboBox->currentText());
    ui->leName->setText(ui->comboBox->currentText());
    ui->lePath->setText(m_settings->path().remove("/sekura/api/v1.0"));
    qDebug() << m_settings->headerValue("sekura-application");
    ui->leApp->setText(m_settings->headerValue("sekura-application"));
    ui->cbAuth->setChecked(m_settings->headers().contains("Authorization"));
    QSettings settings("JupiterSoft", "libSekura");
    if (settings.contains("Default"))
        ui->cbDefault->setChecked(settings.value("Default").toString() == ui->leName->text());
    else
        ui->cbDefault->setChecked(false);
}

void AuthWidget::pb_save() {
    m_settings->setPath(ui->lePath->text() + "/sekura/api/v1.0");
    m_settings->removeHeaderValue("sekura-application");
    m_settings->setHeaderValue("sekura-application", ui->leApp->text().toUtf8());
    if (ui->cbDefault->isChecked()) {
        QSettings settings("JupiterSoft", "libSekura");
        settings.setValue("Default", ui->leName->text());
    }
    m_settings->save(ui->leName->text());
    if (ui->comboBox->currentText() != ui->leName->text())
        ui->comboBox->addItem(ui->leName->text());
}

void AuthWidget::pb_auth() {
    m_settings->removeHeaderValue("Authorization");
    m_settings->setPath(ui->lePath->text() + "/sekura/api/v1.0");
    m_settings->removeHeaderValue("sekura-application");
    m_settings->setHeaderValue("sekura-application", ui->leApp->text().toUtf8());
    AuthDialog *ptr = new AuthDialog;
    if (ptr->exec() == QDialog::Accepted) {
        ui->cbAuth->setChecked(m_settings->headers().contains("Authorization"));
    }
    delete ptr;
}

void AuthWidget::pb_del() {
    QSettings settings("JupiterSoft", "libSekura");
    settings.remove(ui->comboBox->currentText());
    settings.sync();
    reload();
}

void AuthWidget::reload() {
    QSettings settings("JupiterSoft", "libSekura");
    QStringList keys = settings.allKeys(), lst;
    foreach (QString key, keys) {
        if (key == "Default")
            continue;
        QByteArray array = qUncompress(settings.value(key).toByteArray());
        if (!array.isNull())
            lst << key;
    }
    ui->comboBox->clear();
    ui->comboBox->addItems(lst);
    if (lst.size() > 0)
        cmb_index(0);
}
