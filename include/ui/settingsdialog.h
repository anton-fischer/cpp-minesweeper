#pragma once

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    bool getSuccess() const;

private slots:
    void on_buttonBox_accepted();

    void on_input_difficulty_currentTextChanged(const QString &text);

    void on_input_boardSizeX_valueChanged(int value);
    void on_input_boardSizeY_valueChanged(int value);
    void on_input_bombCount_valueChanged(int value);

private:
    Ui::SettingsDialog *ui;

    void updateBombCountLimit();

    bool success = false;
};
