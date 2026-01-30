#pragma once

class Settings
{
public:
    // singleton
    static Settings& instance() {
        static Settings instance;
        return instance;
    }

    // updates settings and returns true for valid input, false for invalid input
    // nothing is changed when false returned
    bool setSettings(const unsigned int boardSizeX, const unsigned int boardSizeY, const unsigned int bombCount);

    unsigned int getBoardSizeX() const;
    unsigned int getBoardSizeY() const;
    unsigned int getBombCount() const;

private:
    // private constructor
    Settings() = default;
    ~Settings() = default;

    // forbid copying
    Settings(const Settings&) = delete;
    Settings& operator=(const Settings&) = delete;

    unsigned int boardSizeX = 10;
    unsigned int boardSizeY = 10;
    unsigned int bombCount = 10;
};
