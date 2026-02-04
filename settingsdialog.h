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

private:
    Ui::SettingsDialog *ui;

    bool success = false;
};
