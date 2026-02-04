#include "settingsDialog.h"
#include "ui_settingsDialog.h"

#include <QMessageBox>
#include <QStringList>

#include "settings.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Configure Game");

    static_assert(DIFFICULTY_ENUM_GUARD == static_cast<int>(Difficulty::_END),  "Difficulty enum version mismatch");
    ui->input_difficulty->addItems(QStringList{"EASY", "MEDIUM", "HARD", "CUSTOM"});
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    const bool validInput = Settings::instance().setSettings(ui->input_boardSizeX->value(), ui->input_boardSizeY->value(), ui->input_bombCount->value());

    if (!validInput) {
        QMessageBox::warning(this, "Invalid Input", "Please check your input");
        return;
    }

    success = true;
}

void SettingsDialog::on_input_difficulty_currentTextChanged(const QString &text)
{
    qDebug() << "Current difficulty:" << text;

    const Difficulty& currentSelection = Settings::stringToDifficulty(text.toStdString());

    static_assert(DIFFICULTY_ENUM_GUARD == static_cast<int>(Difficulty::_END),  "Difficulty enum version mismatch");

    ui->input_boardSizeX->setDisabled(currentSelection != Difficulty::CUSTOM);
    ui->input_boardSizeY->setDisabled(currentSelection != Difficulty::CUSTOM);
    ui->input_bombCount->setDisabled(currentSelection != Difficulty::CUSTOM);

    switch(currentSelection) {
    case (Difficulty::EASY): {
        ui->input_boardSizeX->setValue(9);
        ui->input_boardSizeY->setValue(9);
        ui->input_bombCount->setValue(10);
        break;
    }
    case (Difficulty::MEDIUM): {
        ui->input_boardSizeX->setValue(16);
        ui->input_boardSizeY->setValue(16);
        ui->input_bombCount->setValue(40);
        break;
    }
    case (Difficulty::HARD): {
        ui->input_boardSizeX->setValue(30);
        ui->input_boardSizeY->setValue(16);
        ui->input_bombCount->setValue(99);
        break;
    }
    case (Difficulty::CUSTOM): {
        break;
    }
    default: assert(false);
    }

}

bool SettingsDialog::getSuccess() const {
    return this->success;
}
