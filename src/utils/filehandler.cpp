#include "utils/filehandler.h"

#include <QFile>
#include <QString>
#include <QDebug>
#include <QFileInfo>

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

    QFileInfo info(file);
    qDebug() << "Successfully saved board: " << *board << " at location: " << info.absoluteFilePath();
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

    QFileInfo info(file);
    qDebug() << "Successfully saved player: " << *player << " at location: " << info.absoluteFilePath();
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

        qDebug() << "Successfully loaded player: " << *player;
        return player;
    }
    catch (const std::exception& e) {
        qDebug() << "JSON parse error while loading player:" << e.what();
        return nullptr;
    }
}

std::unique_ptr<Board> FileHandler::createBoardFromFile(std::string filepath) {
    QFile file(QString::fromStdString(filepath));
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error when opening file to load board";
        return nullptr;
    }

    QByteArray rawData = file.readAll();
    file.close();

    try {
        json j = json::parse(rawData.constData());

        auto board = Board::fromJson(j);

        qDebug() << "Successfully loaded board: " << *board;
        return board;
    }
    catch (const std::exception& e) {
        qDebug() << "JSON parse error while loading board:" << e.what();
        return nullptr;
    }
}
