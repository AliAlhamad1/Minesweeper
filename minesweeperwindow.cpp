/**
 * @file minesweeperwindow.cpp
 * @brief Implements the MinesweeperWindow class, which manages the UI and user interaction.
 */

#include "minesweeperwindow.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <queue>

/**
 * @struct Point
 * @brief A small struct to store row/column coordinates for BFS.
 */
struct Point {
    int r; ///< Row index
    int c; ///< Column index
};

/**
 * @brief Constructs a MinesweeperWindow and initializes the game board.
 * @param parent Optional parent widget.
 */
MinesweeperWindow::MinesweeperWindow(QWidget *parent)
    : QWidget(parent),
      m_rows(16),        // Default 16 rows
      m_cols(30),        // Default 30 columns
      m_mineCount(99)    // Default to 99 mines
{
    // Create a new Board instance with the desired dimensions and mine count.
    m_board = new Board(m_cols, m_rows, m_mineCount, this);
    m_board->initialize();

    // Set up the UI (grid of QPushButton cells) and relevant connections.
    setupUI();
}

/**
 * @brief Destructor.
 *
 * Since m_board is a child of this window (QObject hierarchy),
 * it will be cleaned up automatically.
 */
MinesweeperWindow::~MinesweeperWindow()
{
    // Nothing special to do here.
}

/**
 * @brief Sets up the grid layout of buttons representing each cell on the board.
 *
 * Each QPushButton is associated with a (row, col) via dynamic properties,
 * and an event filter is installed to detect right-clicks.
 */
void MinesweeperWindow::setupUI()
{
    m_layout = new QGridLayout();
    m_layout->setSpacing(0);
    m_layout->setMargin(0);

    m_buttons.resize(m_rows);
    for (int r = 0; r < m_rows; ++r) {
        m_buttons[r].resize(m_cols);
        for (int c = 0; c < m_cols; ++c) {
            QPushButton* btn = new QPushButton(this);
            btn->setFixedSize(24, 24);
            btn->setProperty("row", r);
            btn->setProperty("col", c);

            m_layout->addWidget(btn, r, c);
            m_buttons[r][c] = btn;

            // Install event filter to detect right-click.
            btn->installEventFilter(this);
        }
    }

    setLayout(m_layout);
    setupConnections();
}

/**
 * @brief Connects each button's clicked signal to the onCellLeftClicked() slot for handling left-clicks.
 */
void MinesweeperWindow::setupConnections()
{
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            connect(m_buttons[r][c], &QPushButton::clicked,
                    this, &MinesweeperWindow::onCellLeftClicked);
        }
    }
}

/**
 * @brief Event filter to detect right-clicks on cell buttons.
 *
 * Qt QPushButton doesn't expose a rightClicked() signal, so we override
 * eventFilter() to watch for MouseButtonPress events with button == Qt::RightButton.
 *
 * @param obj   The object receiving the event (potentially one of our cell buttons).
 * @param event The event itself (e.g., mouse press).
 * @return True if this event was handled, false otherwise.
 */
bool MinesweeperWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::RightButton) {
            for (int r = 0; r < m_rows; ++r) {
                for (int c = 0; c < m_cols; ++c) {
                    if (obj == m_buttons[r][c]) {
                        MarkState st = m_board->markState(r, c);
                        switch (st) {
                        case MarkState::None:
                            m_board->setMarkState(r, c, MarkState::Flag);
                            break;
                        case MarkState::Flag:
                            m_board->setMarkState(r, c, MarkState::Question);
                            break;
                        case MarkState::Question:
                            m_board->setMarkState(r, c, MarkState::None);
                            break;
                        }
                        updateButtonAppearance(r, c);
                        return true;
                    }
                }
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

/**
 * @brief Slot called when a cell is left-clicked.
 *
 * This method identifies which cell was clicked via the sender(),
 * and then calls revealCell() unless the cell is already revealed or flagged.
 */
void MinesweeperWindow::onCellLeftClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) {
        return;
    }

    int row = btn->property("row").toInt();
    int col = btn->property("col").toInt();

    // Don't reveal if it's already revealed or flagged.
    if (m_board->isRevealed(row, col) ||
        m_board->markState(row, col) == MarkState::Flag) {
        return;
    }

    revealCell(row, col);
}

/**
 * @brief Reveals the cell at (row, col). If it's a mine, the game ends.
 *        Otherwise, updates the UI and cascades if it's a zero-neighbor cell.
 *
 * @param row The row index of the cell to reveal.
 * @param col The column index of the cell to reveal.
 */
void MinesweeperWindow::revealCell(int row, int col)
{
    if (m_board->isMine(row, col)) {
        m_board->setRevealed(row, col, true);
        updateButtonAppearance(row, col);
        gameOver(false);
        return;
    }

    m_board->setRevealed(row, col, true);
    updateButtonAppearance(row, col);

    if (m_board->neighborMineCount(row, col) == 0) {
        cascadeReveal(row, col);
    }

    checkWinCondition();
}

/**
 * @brief Performs a BFS cascade from a zero-neighbor cell to reveal adjacent safe cells.
 *
 * @param startRow Row index of the origin cell.
 * @param startCol Column index of the origin cell.
 */
void MinesweeperWindow::cascadeReveal(int startRow, int startCol)
{
    std::queue<Point> q;
    q.push({startRow, startCol});

    while (!q.empty()) {
        Point p = q.front();
        q.pop();

        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0 && dc == 0) {
                    continue;
                }
                int rr = p.r + dr;
                int cc = p.c + dc;
                if (!m_board->inBounds(rr, cc)) {
                    continue;
                }

                if (!m_board->isRevealed(rr, cc) &&
                    m_board->markState(rr, cc) != MarkState::Flag)
                {
                    m_board->setRevealed(rr, cc, true);
                    updateButtonAppearance(rr, cc);

                    if (m_board->neighborMineCount(rr, cc) == 0) {
                        q.push({rr, cc});
                    }
                }
            }
        }
    }
}

/**
 * @brief Updates the appearance of the button at (row, col) based on the cell state.
 *
 * This may include showing a mine icon, a flagged icon, a number, or nothing.
 *
 * @param row Row index of the cell.
 * @param col Column index of the cell.
 */
void MinesweeperWindow::updateButtonAppearance(int row, int col)
{
    QPushButton* btn = m_buttons[row][col];
    if (!btn) {
        return;
    }

    if (m_board->isRevealed(row, col)) {
        if (m_board->isMine(row, col)) {
            btn->setIcon(QIcon(":/mine.png"));
            btn->setIconSize(QSize(24, 24));
            btn->setEnabled(false);
        } else {
            int count = m_board->neighborMineCount(row, col);
            if (count > 0) {
                btn->setText(QString::number(count));
            } else {
                btn->setText("");
            }
            btn->setEnabled(false);
            btn->setStyleSheet("background-color: #d0d0d0;");
        }
    } else {
        MarkState st = m_board->markState(row, col);
        switch (st) {
        case MarkState::None:
            btn->setIcon(QIcon());
            btn->setText("");
            btn->setEnabled(true);
            btn->setStyleSheet("");
            break;
        case MarkState::Flag:
            btn->setIcon(QIcon(":/flag.png"));
            btn->setIconSize(QSize(24, 24));
            btn->setText("");
            btn->setEnabled(true);
            break;
        case MarkState::Question:
            btn->setIcon(QIcon(":/question.png"));
            btn->setIconSize(QSize(24, 24));
            btn->setText("");
            btn->setEnabled(true);
            break;
        }
    }
}

/**
 * @brief Checks if all non-mine cells are revealed; if so, end the game with a win.
 */
void MinesweeperWindow::checkWinCondition()
{
    if (m_board->allNonMinesRevealed()) {
        gameOver(true);
    }
}

/**
 * @brief Ends the game, revealing mines if lost, then prompting the user to restart or quit.
 *
 * @param won True if the player won; false otherwise (stepped on a mine).
 */
void MinesweeperWindow::gameOver(bool won)
{
    if (!won) {
        for (int r = 0; r < m_rows; ++r) {
            for (int c = 0; c < m_cols; ++c) {
                if (m_board->isMine(r, c)) {
                    m_board->setRevealed(r, c, true);
                    updateButtonAppearance(r, c);
                }
            }
        }
    }

    QString title = won ? "Congratulations!" : "Game Over";
    QString text  = won ? "You Win!" : "You hit a mine!";

    QMessageBox::StandardButton reply =
        QMessageBox::question(this,
                              title,
                              text + "\nPlay again?",
                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        resetGame();
    } else {
        close();
    }
}

/**
 * @brief Resets the board and button appearances for a new game session.
 */
void MinesweeperWindow::resetGame()
{
    m_board->initialize();
    for (int r = 0; r < m_rows; ++r) {
        for (int c = 0; c < m_cols; ++c) {
            QPushButton* btn = m_buttons[r][c];
            btn->setIcon(QIcon());
            btn->setText("");
            btn->setEnabled(true);
            btn->setStyleSheet("");
        }
    }
}
