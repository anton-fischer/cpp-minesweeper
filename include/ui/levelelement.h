#pragma once

#include <QWidget>

class Player;

QT_BEGIN_NAMESPACE
namespace Ui {
class LevelElement;
}
QT_END_NAMESPACE

class LevelElement : public QWidget
{
    Q_OBJECT

public:
    explicit LevelElement(QWidget* parent = nullptr);
    LevelElement(Player* player, QWidget* parent = nullptr);
    ~LevelElement();

    void updatePlayer(Player* newPlayer);

private slots:
    void playerLevelUp();
    void playerXpChange();

private:
    Ui::LevelElement *ui;

    Player* player;
};
