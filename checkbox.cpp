/*
 * © 2023
 * Author: Akhat T. Kuangaliyev
 * Company: Jupiter Soft
 */
#include "checkbox.h"
#include "ui_checkbox.h"

namespace Sekura {

    CheckBox::CheckBox(int type, QWidget *parent) : BaseItem(type, parent), ui(new Ui::CheckBox) {
        ui->setupUi(this);
    }

    CheckBox::~CheckBox() { delete ui; }

    void CheckBox::setValue(const QVariant &dt) { ui->checkBox->setChecked(dt.toBool()); }

    QVariant CheckBox::value() const { return ui->checkBox->isChecked(); }

    void CheckBox::setCaption(const QString &str) { ui->checkBox->setText(str); }

    void CheckBox::setReadOnly() { setDisabled(true); }

} // namespace Sekura
