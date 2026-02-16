#include "ui/settingsDialog.h"
#include "ui_settingsDialog.h"

#include <QMessageBox>
#include <QStringList>

#include "core/settings.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Configure Game");

    connect(this, &QDialog::rejected, this, []() {
        qDebug() << "Dialog was closed via X";
        // TODO
    });

    static_assert(DIFFICULTY_ENUM_GUARD == static_cast<int>(Difficulty::_END),  "Difficulty enum version mismatch");
    ui->input_difficulty->addItems(QStringList{"EASY", "MEDIUM", "HARD", "CUSTOM"});
    ui->input_difficulty->setCurrentText(QString::fromStdString(Settings::difficultyToString(Settings::instance().getDifficulty())));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    bool validInput = false;
    if (ui->input_difficulty->currentText() == "CUSTOM") validInput = Settings::instance().setSettings(ui->input_boardSizeX->value(), ui->input_boardSizeY->value(), ui->input_bombCount->value());
    else                                                 validInput = Settings::instance().setSettings(Settings::stringToDifficulty(ui->input_difficulty->currentText().toStdString()));

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

    auto& difficultyData = DIFFICULTY_DATA_TABLE[static_cast<int>(currentSelection)];
    ui->input_boardSizeX->setValue(difficultyData.boardSizeX);
    ui->input_boardSizeY->setValue(difficultyData.boardSizeY);
    ui->input_bombCount->setValue(difficultyData.bombCount);

}

bool SettingsDialog::getSuccess() const {
    return this->success;
}
