#pragma once

#include <QDebug>
#include <memory>

#include "utils/json.hpp"

class Tile
{
public:
    Tile();
    ~Tile() = default;

    bool getIsBomb() const;
    bool getIsFlag() const;
    bool getIsCovered() const;

    unsigned int getNumber() const;

    void setIsBomb(bool isBomb);
    void setIsFlag(bool isFlag);
    void setIsCovered(bool isCovered);

    void setNumber(unsigned int number);
    void incrementNumber();

    // persistor
    static std::unique_ptr<Tile> fromJson(const nlohmann::json& j);
    nlohmann::json toJson() const;

    // toString
    friend QDebug operator<<(QDebug dbg, const Tile& t);

private:
    bool isBomb = false;
    bool isFlag = false;
    bool isCovered = true;

    unsigned int number = 0;
};
