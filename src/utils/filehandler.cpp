#include "utils/filehandler.h"

#include <QFile>
#include <QString>
#include <QDebug>
#include <QFileInfo>

#include "core/board.h"
#include "core/player.h"
#include "core/settings.h"

FileHandler::FileHandler() {}

bool FileHandler::saveSettings(std::string filepath) {
    json j = Settings::instance().toJson();

    QFile file(QString::fromStdString(filepath));
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Error when opening file to save settings";
        return false;
    }

    QString output = QString::fromStdString(j.dump(4)); // indent is 4
    file.write(output.toUtf8());
    file.close();

    QFileInfo info(file);
    qDebug() << "Successfully saved settings at location: " << info.absoluteFilePath();
    return true;
}

bool FileHandler::loadSettings(std::string filepath) {
    QFile file(QString::fromStdString(filepath));
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error when opening file to load settings";
        return false;
    }

    QByteArray rawData = file.readAll();
    file.close();

    try {
        json j = json::parse(rawData.constData());

        Settings::fromJson(j);

        qDebug() << "Successfully loaded settings";
        return true;
    }
    catch (const std::exception& e) {
        qDebug() << "JSON parse error while loading settings:" << e.what();
        return false;
    }
}

bool FileHandler::saveBoardAsFile(Board* board, std::string filepath) {
    json j = board->toJson();

    QFile file(QString::fromStdString(filepath));
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Error when opening file to save board";
        return false;
    }

    QString output = QString::fromStdString(j.dump(4)); // indent is 4
    file.write(output.toUtf8());
    file.close();

    QFileInfo info(file);
    qDebug() << "Successfully saved board: " << *board << " at location: " << info.absoluteFilePath();
    return true;
}

bool FileHandler::savePlayerAsFile(Player* player, std::string filepath) {
    json j = player->toJson();

    QFile file(QString::fromStdString(filepath));
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Error when opening file to save player";
        return false;
    }

    QString output = QString::fromStdString(j.dump(4)); // indent is 4
    file.write(output.toUtf8());
    file.close();

    QFileInfo info(file);
    qDebug() << "Successfully saved player: " << *player << " at location: " << info.absoluteFilePath();
    return true;
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
