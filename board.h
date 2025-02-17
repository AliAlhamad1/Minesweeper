#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <QVector>
#include "cell.h"

/**
 * @class Board
 * @brief Manages the state of the Minesweeper grid, including mine placement and cell statuses.
 *
 * The Board class stores a 2D grid of Cell objects and provides methods to
 * initialize the board, query or update individual cells, and check overall
 * game progress (e.g., whether all non-mine cells are revealed).
 */
class Board : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a Board with the given dimensions and mine count.
     * @param width      Number of columns (horizontal size).
     * @param height     Number of rows (vertical size).
     * @param mineCount  Number of mines to randomly place.
     * @param parent     Optional parent QObject for memory management.
     */
    Board(int width, int height, int mineCount, QObject *parent = nullptr);

    /**
     * @brief Initializes or resets the board by clearing cells and randomly placing mines.
     */
    void initialize();

    /**
     * @brief Checks if the given (row, col) coordinates are within board boundaries.
     * @param row  Row index to check.
     * @param col  Column index to check.
     * @return True if (row, col) is inside the board; false otherwise.
     */
    bool inBounds(int row, int col) const;

    /**
     * @brief Checks whether the cell at (row, col) contains a mine.
     * @param row  Row index of the cell.
     * @param col  Column index of the cell.
     * @return True if a mine is present, false otherwise.
     */
    bool isMine(int row, int col) const;

    /**
     * @brief Sets whether there is a mine at the cell (row, col).
     * @param row    Row index of the cell.
     * @param col    Column index of the cell.
     * @param isMine True if the cell should have a mine; false otherwise.
     */
    void setMine(int row, int col, bool isMine);

    /**
     * @brief Checks if the cell at (row, col) is revealed.
     * @param row  Row index of the cell.
     * @param col  Column index of the cell.
     * @return True if the cell has been revealed, false otherwise.
     */
    bool isRevealed(int row, int col) const;

    /**
     * @brief Sets whether the cell at (row, col) is revealed.
     * @param row  Row index of the cell.
     * @param col  Column index of the cell.
     * @param r    True to mark as revealed, false otherwise.
     */
    void setRevealed(int row, int col, bool r);

    /**
     * @brief Retrieves the mark state (None, Flag, Question) of the cell at (row, col).
     * @param row  Row index of the cell.
     * @param col  Column index of the cell.
     * @return The current MarkState for that cell.
     */
    MarkState markState(int row, int col) const;

    /**
     * @brief Sets the mark state (Flag, Question, or None) for the cell at (row, col).
     * @param row   Row index of the cell.
     * @param col   Column index of the cell.
     * @param state The new mark state to assign.
     */
    void setMarkState(int row, int col, MarkState state);

    /**
     * @brief Computes how many mines are adjacent to (row, col).
     *
     * This includes the 8 neighboring cells around (row, col): horizontal,
     * vertical, and diagonal.
     *
     * @param row  Row index of the cell.
     * @param col  Column index of the cell.
     * @return Number of neighboring mines (0 to 8).
     */
    int neighborMineCount(int row, int col) const;

    /**
     * @brief Returns the total width (number of columns) of the board.
     */
    int width() const    { return m_width; }

    /**
     * @brief Returns the total height (number of rows) of the board.
     */
    int height() const   { return m_height; }

    /**
     * @brief Checks if all non-mine cells are revealed.
     * @return True if every safe cell is revealed; false otherwise.
     */
    bool allNonMinesRevealed() const;

    /**
     * @brief Determines if the cell at (row, col) is revealed and has 0 neighboring mines.
     * @return True if the cell is in-bounds, revealed, and neighborMineCount == 0.
     */
    bool isClearedCell(int row, int col) const;

private:
    int m_width;     ///< Number of columns in the board.
    int m_height;    ///< Number of rows in the board.
    int m_mineCount; ///< Total number of mines placed in the board.

    /**
     * @brief 2D container for the Cells that make up the Minesweeper board.
     *
     * Indexed as m_cells[row][col], where row ranges from 0..m_height-1
     * and col ranges from 0..m_width-1.
     */
    QVector<QVector<Cell>> m_cells;
};

#endif // BOARD_H
