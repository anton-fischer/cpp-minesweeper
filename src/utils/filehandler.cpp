#include "utils/filehandler.h"

#include <QFile>
#include <QString>
#include <QDebug>

#include "core/board.h"
#include "core/player.h"

FileHandler::FileHandler() {}

void FileHandler::saveBoardAsFile(Board* board, std::string filepath) {
    json j = board->toJson();

    QFile file(QString::fromStdString(filepath));
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Error when opening file to save board";
        return;
    }

    QString output = QString::fromStdString(j.dump(4)); // indent is 4
    file.write(output.toUtf8());
    file.close();

    qDebug() << "Successfully saved board: " << file.fileName();
}

void FileHandler::savePlayerAsFile(Player* player, std::string filepath) {
    json j = player->toJson();

    QFile file(QString::fromStdString(filepath));
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Error when opening file to save player";
        return;
    }

    QString output = QString::fromStdString(j.dump(4)); // indent is 4
    file.write(output.toUtf8());
    file.close();

    qDebug() << "Successfully saved player: " << file.fileName();
}

std::unique_ptr<Player> FileHandler::createPlayerFromFile(std::string filepath) {
    QFile file(QString::fromStdString(filepath));
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error when opening file to load player";
        return nullptr;
    }

    QByteArray rawData = file.readAll();
    file.close();

    try {
        json j = json::parse(rawData.constData());

        auto player = Player::fromJson(j);

        qDebug() << "Successfully loaded player";
        return player;
    }
    catch (const std::exception& e) {
        qDebug() << "JSON parse error while loading player:" << e.what();
        return nullptr;
    }
}

std::unique_ptr<Board> FileHandler::createBoardFromFile(std::string filepath) {
    Board* b = new Board();

    // TODO implement logic

    qDebug() << "Successfully loaded board";
    return std::make_unique<Board>(b);
}
