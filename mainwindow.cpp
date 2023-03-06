/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "mainwindow.h"
#include "basewidget.h"
#include "menu.h"
#include "restsettings.h"
#include "ui_mainwindow.h"

#include <QMdiSubWindow>
#include <QTimer>

using namespace Sekura;

MainWindow::MainWindow(const QString &conf, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), m_conf(conf) {
    ui->setupUi(this);
    ui->mdiArea->setActivationOrder(QMdiArea::ActivationHistoryOrder);
    m_settings = new RestSettings(this);
    if (!m_settings->load(m_conf)) {
        m_error = true;
    }
    Sekura::Interface::setSettings(m_settings);
    Menu *menu = Interface::createMenu(ui->menubar, this);
    connect(menu, &Menu::childCreated, this, &MainWindow::appendWidget);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::appendWidget(BaseWidget *widget) {
    widget->setParent(this);
    QTimer::singleShot(100, this, [this, widget]() {
        QMdiSubWindow *w = ui->mdiArea->addSubWindow(widget);
        w->setAttribute(Qt::WA_DeleteOnClose);
        w->show();
        connect(widget, &BaseWidget::appendWidget, this, &MainWindow::appendWidget);
        connect(widget, &BaseWidget::closeParent, w, &QMdiSubWindow::close);
    });
}
