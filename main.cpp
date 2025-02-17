/**
 * @file main.cpp
 * @brief Entry point for the Minesweeper application. Initializes Qt and shows the main window.
 */

#include <QApplication>
#include "minesweeperwindow.h"

/**
 * @brief Main function that creates the application and shows the Minesweeper game window.
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return The application's exit code.
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create the main game window
    MinesweeperWindow window;
    window.setWindowTitle("Minesweeper (Qt 5)");
    window.show();

    // Start the Qt event loop
    return app.exec();
}
