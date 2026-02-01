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

private:
    Ui::SettingsDialog *ui;

    bool success = false;
};
