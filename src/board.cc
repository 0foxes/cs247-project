#include "../includes/board.h"

Board::Board() : grid{vector<vector<Cell>>(8, vector<Cell>(8))} {}

void Board::registerObserver(shared_ptr<View> observer) { observers.push_back(observer); }

void Board::init() {
    // init server ports, otherwise all cells default empty
    grid.at(0).at(3).isServerPort = true;
    notifyObservers(0, 3, 'S');
    grid.at(0).at(4).isServerPort = true;
    notifyObservers(0, 4, 'S');
    grid.at(7).at(3).isServerPort = true;
    notifyObservers(7, 3, 'S');
    grid.at(7).at(4).isServerPort = true;
    notifyObservers(7, 4, 'S');
}

void Board::notifyObservers(int r, int c, char state) {
    for (shared_ptr<View> observer : observers) {
        observer->notify(r, c, state);
    }
}

// void Board::print(ostream& out) {
//     for (shared_ptr<View> observer : observers) {
//         observer->print(out);
//     }
//     // for (auto i : linkLocs) {
//     //     out << i.first->symbol() << "(" << i.second.first << ", "
//     //         << i.second.second << ") ";
//     // }
//     // out << endl;
// }

void Board::initLink(int r, int c, shared_ptr<Link> link) {
    if (!isInBounds(r, c)) {
        cerr << "Error: board can't init link - out of bounds.\n";
        return;
    }

    Cell& targetCell = grid[r][c];
    if (targetCell.link != nullptr) {
        cerr << "Error: target cell already occupied by a link.\n";
        return;
    }

    targetCell.link = link;
    // notify observers
    notifyObservers(r, c, link->getSymbol());
}

Board::MoveResult Board::moveLink(int r, int c, shared_ptr<Link> link) {
    MoveResult move;
    move.newR = r;
    move.newC = c;
    move.symbol = link->getSymbol();

    if (!isInBounds(r, c)) {
        cerr << "Error: board can't move link - out of bounds.\n";
        return move;
    }

    Cell& targetCell = grid[r][c];

    // link must be on the board to move it!
    pair<int, int> loc = getLinkLocation(link);
    int oldR = loc.first;
    int oldC = loc.second;
    if (oldR == -1 || oldC == -1) {
        cerr << "Error: link not found on the board.\n";
        return move;
    }

    // target cell contains a link already
    if (targetCell.link != nullptr) {
        if (link->getOwner() == targetCell.link->getOwner()) {
            cerr << "Error: target cell occupied by your own link\n";
            return move;
        } else {
            // TODO: battle!!!!!!!!!!
            cout << "Battle between links occurs here.\n";
            return move;
        }
    }

    if (targetCell.isServerPort) {
        // TODO: server port logic??
    }

    if (targetCell.hasFirewall) {
        // TODO: firewall logic??????
    }

    grid[oldR][oldC].link = nullptr;
    move.oldR = oldR;
    move.oldC = oldC;

    // update target cell properties
    targetCell.link = link;
    // notify observers
    notifyObservers(r, c, link->getSymbol());
    notifyObservers(oldR, oldC, '.'); // clear old link

    return move;
}

bool Board::isInBounds(int r, int c) const {
    // assumes grid is rectangle
    return (r >= 0) && (r < grid.size()) && (c >= 0) && (c < grid[0].size());
}

Board::MoveResult Board::moveLink(shared_ptr<Link> link, char direction) {
    // link must be on the board to move it!
    pair<int, int> currLoc = getLinkLocation(link);
    int oldR = currLoc.first;
    int oldC = currLoc.second;
    if (oldR == -1 || oldC == -1) {
        cerr << "Error: link not found on the board.\n";
    }

    // boosted links move 2 cells instead of 1
    int distance = 1;
    if (link->getIsBoosted()) {
        distance = 2;
    }

    // compute the move given the old cell and new direction
    int newRow = oldR;
    int newCol = oldC;

    switch (direction) {
    case 'u':
        newRow -= distance;
        break;
    case 'd':
        newRow += distance;
        break;
    case 'l':
        newCol -= distance;
        break;
    case 'r':
        newCol += distance;
        break;
    default:
        cerr << "invalid direction" << endl;
        return MoveResult{oldR, oldC, -1, -1, link->getSymbol()};
    }

    return moveLink(newRow, newCol, link);
}
