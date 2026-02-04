#include "mainmenu.h"
#include "ui_mainmenu.h"

#include <QToolButton>

#include "mainwindow.h"
#include "settingsdialog.h"
#include "quest.h"

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::appendQuest(Quest& quest) {
    auto* widget = new QWidget(this);
    auto* layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0,0,0,0);

    auto* objectiveLabel = new QLabel(QString::fromStdString(quest.generateObjectiveString()), widget);
    objectiveLabel->setMinimumWidth(100);
    objectiveLabel->setMinimumHeight(30);

    auto* progressBar = new QProgressBar(widget);
    progressBar->setMaximum(quest.getGoal());
    progressBar->setValue(quest.getProgress());

    auto* rerollButton = new QToolButton(widget);
    rerollButton->setMinimumWidth(30);
    rerollButton->setMinimumHeight(30);
    rerollButton->setText("🔄️");

    layout->addWidget(objectiveLabel);
    layout->addWidget(progressBar);
    layout->addWidget(rerollButton);

    ui->quests_container->addWidget(widget);
}

void MainMenu::on_btn_save_clicked()
{
    Quest q;
    appendQuest(q);
}

void MainMenu::on_btn_exit_clicked()
{
    QCoreApplication::quit();
}

void MainMenu::on_btn_play_clicked()
{
    MainWindow* window = new MainWindow();
    window->show();
    this->close();
}

void MainMenu::on_btn_configure_clicked()
{
    SettingsDialog dialog(this);
    dialog.exec();
}
