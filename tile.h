#pragma once

class Tile
{
public:
    Tile();

    bool getIsBomb() const;
    bool getIsFlag() const;
    bool getIsCovered() const;

    unsigned int getNumber() const;

    void setIsBomb(bool isBomb);
    void setIsFlag(bool isFlag);
    void setIsCovered(bool isCovered);

    void setNumber(unsigned int number);
    void incrementNumber();

private:
    bool isBomb = false;
    bool isFlag = false;
    bool isCovered = true;

    unsigned int number = 0;
};
