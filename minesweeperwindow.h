#ifndef MINESWEEPERWINDOW_H
#define MINESWEEPERWINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QVector>
#include <QPushButton>
#include "board.h"

/**
 * @class MinesweeperWindow
 * @brief The main UI window for the Minesweeper application.
 *
 * This class creates and manages the grid of QPushButtons representing cells.
 * It also handles user input (left-clicks, right-clicks) and communicates
 * with the Board class to update game state and check for win/loss conditions.
 */
class MinesweeperWindow : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a MinesweeperWindow with the specified parent widget.
     * @param parent Optional parent for Qt's memory management.
     */
    explicit MinesweeperWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor for MinesweeperWindow.
     */
    ~MinesweeperWindow();

protected:
    /**
     * @brief Event filter to catch right-clicks on the buttons representing cells.
     *
     * We override this to detect Qt::RightButton presses, as QPushButton
     * does not directly support right-click signals.
     *
     * @param obj   The object receiving the event.
     * @param event The event itself.
     * @return True if the event was handled here, false otherwise.
     */
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    /**
     * @brief Slot triggered when a cell is left-clicked.
     *
     * Determines which cell was clicked and calls revealCell() if appropriate.
     */
    void onCellLeftClicked();

private:
    Board *m_board;               ///< Pointer to the Board managing mines and cell states.
    QGridLayout *m_layout;        ///< Layout to arrange cell buttons in a 2D grid.
    QVector<QVector<QPushButton*>> m_buttons; ///< 2D array of pointers to QPushButtons.

    int m_rows;                   ///< Number of rows in the grid (default 16).
    int m_cols;                   ///< Number of columns in the grid (default 30).
    int m_mineCount;              ///< Number of mines to place (default 99).

    /**
     * @brief Sets up the UI elements (button grid) and layout.
     */
    void setupUI();

    /**
     * @brief Connects signals and slots for button clicks and sets up event filters.
     */
    void setupConnections();

    /**
     * @brief Resets the board and the UI for a new game.
     */
    void resetGame();

    /**
     * @brief Reveals the cell at the given row and column.
     *
     * If the cell is a mine, triggers a game over. Otherwise, reveals the cell
     * and possibly cascades if it's a 0-neighbor cell.
     *
     * @param row The row of the clicked cell.
     * @param col The column of the clicked cell.
     */
    void revealCell(int row, int col);

    /**
     * @brief Cascades reveals from an empty cell (0 neighboring mines).
     *
     * Performs a breadth-first search (BFS) to reveal
     * all connected empty cells, plus their immediate neighbors.
     *
     * @param startRow The row of the origin cell.
     * @param startCol The column of the origin cell.
     */
    void cascadeReveal(int startRow, int startCol);

    /**
     * @brief Updates a single button's icon, text, and state based on its underlying cell.
     *
     * @param row Row index of the cell/button.
     * @param col Column index of the cell/button.
     */
    void updateButtonAppearance(int row, int col);

    /**
     * @brief Checks if all non-mine cells have been revealed (win condition).
     */
    void checkWinCondition();

    /**
     * @brief Ends the game, either as a win or a loss, and prompts to restart or quit.
     *
     * @param won True if the player won, false if lost.
     */
    void gameOver(bool won);
};

#endif // MINESWEEPERWINDOW_H
