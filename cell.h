#ifndef CELL_H
#define CELL_H

/**
 * @enum MarkState
 * @brief Represents the marking status of a cell (none, flagged, or question).
 */
enum class MarkState {
    None,       ///< No mark at all
    Flag,       ///< Flag mark for suspected mine
    Question    ///< Question mark for uncertain cases
};

/**
 * @class Cell
 * @brief Stores the state of an individual cell in the Minesweeper grid.
 *
 * A Cell can be a mine or not, can be revealed or hidden,
 * and can have a mark (flag, question mark, or none).
 */
class Cell
{
public:
    /**
     * @brief Default constructor for a Cell.
     *
     * Initializes a cell with no mine, not revealed, and no mark.
     */
    Cell()
        : m_hasMine(false)
        , m_revealed(false)
        , m_mark(MarkState::None)
    {
        // Nothing else to do here
    }

    /**
     * @brief Checks if this cell contains a mine.
     * @return True if the cell is mined, false otherwise.
     */
    bool hasMine() const { return m_hasMine; }

    /**
     * @brief Sets whether this cell contains a mine.
     * @param hasMine True if this cell should have a mine.
     */
    void setMine(bool hasMine) { m_hasMine = hasMine; }

    /**
     * @brief Checks if this cell has been revealed to the player.
     * @return True if revealed, false otherwise.
     */
    bool isRevealed() const { return m_revealed; }

    /**
     * @brief Sets whether this cell is revealed.
     * @param r True if revealed, false otherwise.
     */
    void setRevealed(bool r) { m_revealed = r; }

    /**
     * @brief Returns the current marking state (None, Flag, or Question).
     */
    MarkState markState() const { return m_mark; }

    /**
     * @brief Sets the current marking state (None, Flag, or Question).
     * @param state The desired MarkState.
     */
    void setMarkState(MarkState state) { m_mark = state; }

    /**
     * @brief Resets the cell to its default values (no mine, not revealed, no mark).
     */
    void reset()
    {
        m_hasMine = false;
        m_revealed = false;
        m_mark = MarkState::None;
    }

private:
    bool m_hasMine;      ///< True if a mine is placed in this cell.
    bool m_revealed;     ///< True if this cell is currently revealed.
    MarkState m_mark;    ///< Current mark state of the cell (Flag, Question, or None).
};

#endif // CELL_H
