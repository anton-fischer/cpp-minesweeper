#pragma once

#include <QWidget>

class Quest;

QT_BEGIN_NAMESPACE
namespace Ui {
class QuestElement;
}
QT_END_NAMESPACE

class QuestElement : public QWidget
{
    Q_OBJECT

public:
    explicit QuestElement(Quest* quest, QWidget* parent = nullptr);
    ~QuestElement();

private slots:
    void on_btn_reroll_clicked();

private:
    Ui::QuestElement *ui;

    void updateQuest();

    Quest* quest;
};
