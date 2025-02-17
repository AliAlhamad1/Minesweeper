# Minesweeper (Qt 5 with qmake)

## Overview
A basic Minesweeper clone in C++/Qt, built specifically using **qmake** (Qt’s traditional build system).  

- **Grid Size:** 30×16  
- **Mines:** 99  
- **Controls:**
  - Left-click to reveal a cell.
  - Right-click to cycle between Flag → Question → None.

## Build Instructions
1. **qmake Minesweeper.pro**  
   - You may see some warnings appear after running `qmake`. **Ignore them** and proceed to the next step.
2. **make**  
3. **Run/Launch**:
   - On **macOS**: `open Minesweeper.app`
   - On **Linux**: `./Minesweeper`

## Gameplay & Display
- We recommend playing in **full screen** for better visibility.
- If your system uses **Dark Mode**, please disable it for better contrast with the game board.

## Icons
- Ensure you place `mine.png`, `flag.png`, and `question.png` inside an `images/` folder (referenced by `resources.qrc`).

Enjoy playing Minesweeper!
