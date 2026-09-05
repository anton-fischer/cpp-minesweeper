# Minesweeper

A desktop Minesweeper game built with C++17 and Qt Widgets, featuring player
profiles, XP/leveling, quests, highscores, and save/load support for boards
and players.

## Features

- Classic Minesweeper gameplay with Easy, Medium, Hard, and Custom difficulties
- Player profiles with XP, levels, and quest progress
- Highscore tracking per difficulty
- Save and load board or player state to/from JSON files
- Light and dark themes

## Requirements

- CMake 3.16+
- Qt 6 (or Qt 5) with the Widgets component
- A C++17 compiler (tested with MinGW on Windows)

## Building

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="<path-to-your-Qt-installation>"
cmake --build build
```

The resulting `minesweeper` executable will be placed in the build directory.

## Project structure

```
include/    Public headers (core, ui, utils)
src/        Implementation files and Qt Designer .ui forms
resources/  Icons, images, and stylesheets bundled via resources.qrc
```

## Third-party libraries

- [nlohmann/json](https://github.com/nlohmann/json) (MIT License) — bundled at [include/utils/json.hpp](include/utils/json.hpp)
- Icon by [Icons8](https://icons8.com)

## License

This project is licensed under the [MIT License](LICENSE).
