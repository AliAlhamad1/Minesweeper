//
// board.cpp
//
// This file contains the implementation of the Board class, which
// manages the internal state of the Minesweeper grid, randomizes
// mine placement, and provides methods for inspecting and updating
// each cell.
//

#include "board.h"
#include <cstdlib>  // For std::rand, std::srand
#include <ctime>    // For std::time

/**
 * @brief Constructs a Board object with the specified dimensions and mine count.
 *
 * @param width      The number of columns (horizontal size).
 * @param height     The number of rows (vertical size).
 * @param mineCount  The number of mines to place on the board.
 * @param parent     An optional parent QObject for Qt's memory management.
 */
Board::Board(int width, int height, int mineCount, QObject *parent)
    : QObject(parent),
      m_width(width),
      m_height(height),
      m_mineCount(mineCount)
{
    // Initialize the 2D vector of Cell objects to match the board dimensions.
    m_cells.resize(m_height);
    for (int row = 0; row < m_height; ++row) {
        m_cells[row].resize(m_width);
    }
}

/**
 * @brief Initializes (or resets) the board by clearing cells and randomly placing mines.
 *
 * Call this method at the start of a new game to reset the board to a clean state.
 */
void Board::initialize()
{
    // 1. Reset all cells to their default state.
    for (int r = 0; r < m_height; ++r) {
        for (int c = 0; c < m_width; ++c) {
            m_cells[r][c].reset();
        }
    }

    // 2. Seed the random number generator and randomly place m_mineCount mines.
    std::srand(std::time(nullptr));
    int placed = 0;
    while (placed < m_mineCount) {
        int rr = std::rand() % m_height;
        int cc = std::rand() % m_width;

        // Only place a mine if none is there yet.
        if (!m_cells[rr][cc].hasMine()) {
            m_cells[rr][cc].setMine(true);
            ++placed;
        }
    }
}

/**
 * @brief Checks if the provided row and column are within board boundaries.
 *
 * @param row The row index to check.
 * @param col The column index to check.
 * @return True if (row, col) is inside the board; false otherwise.
 */
bool Board::inBounds(int row, int col) const
{
    return (row >= 0 && row < m_height && col >= 0 && col < m_width);
}

/**
 * @brief Determines if the cell at (row, col) contains a mine.
 */
bool Board::isMine(int row, int col) const
{
    return m_cells[row][col].hasMine();
}

/**
 * @brief Sets whether there is a mine in the cell at (row, col).
 */
void Board::setMine(int row, int col, bool isMine)
{
    m_cells[row][col].setMine(isMine);
}

/**
 * @brief Checks if the cell at (row, col) is revealed to the player.
 */
bool Board::isRevealed(int row, int col) const
{
    return m_cells[row][col].isRevealed();
}

/**
 * @brief Marks the cell at (row, col) as revealed or unrevealed.
 *
 * @param r True if revealed, false otherwise.
 */
void Board::setRevealed(int row, int col, bool r)
{
    m_cells[row][col].setRevealed(r);
}

/**
 * @brief Returns the marking state (None, Flag, or Question) of the cell at (row, col).
 */
MarkState Board::markState(int row, int col) const
{
    return m_cells[row][col].markState();
}

/**
 * @brief Sets the marking state of the cell at (row, col) to the specified value.
 *
 * @param state The new mark state (None, Flag, or Question).
 */
void Board::setMarkState(int row, int col, MarkState state)
{
    m_cells[row][col].setMarkState(state);
}

/**
 * @brief Counts how many of the eight neighboring cells around (row, col) contain mines.
 *
 * @return The number of adjacent mines.
 */
int Board::neighborMineCount(int row, int col) const
{
    int count = 0;
    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) {
                continue;
            }

            int rr = row + dr;
            int cc = col + dc;
            if (inBounds(rr, cc) && m_cells[rr][cc].hasMine()) {
                ++count;
            }
        }
    }
    return count;
}

/**
 * @brief Checks if all non-mine cells on the board have been revealed.
 *
 * @return True if every safe cell is revealed, false otherwise.
 */
bool Board::allNonMinesRevealed() const
{
    for (int r = 0; r < m_height; ++r) {
        for (int c = 0; c < m_width; ++c) {
            if (!m_cells[r][c].hasMine() && !m_cells[r][c].isRevealed()) {
                return false;
            }
        }
    }
    return true;
}

/**
 * @brief Checks if a cell is revealed and has 0 neighboring mines.
 *
 * @return True if the cell is in-bounds, revealed, and neighborMineCount == 0.
 */
bool Board::isClearedCell(int row, int col) const
{
    if (!inBounds(row, col)) {
        return false;
    }
    if (!isRevealed(row, col)) {
        return false;
    }

    return (neighborMineCount(row, col) == 0);
}
