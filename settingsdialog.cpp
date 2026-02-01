#include "settingsDialog.h"
#include "ui_settingsDialog.h"

#include <QMessageBox>
#include "settings.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    const bool validInput = Settings::instance().setSettings(ui->input_BoardSizeX->value(), ui->input_BoardSizeY->value(), ui->input_BombCount->value());

    if (!validInput) {
        QMessageBox::warning(this, "Invalid Input", "Please check your input");
        return;
    }

    success = true;
}

bool SettingsDialog::getSuccess() const {
    return this->success;
}
