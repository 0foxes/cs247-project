#include "../includes/board.h"

Board::Board() : grid{vector<vector<Cell>>(8, vector<Cell>(8))} {}

void Board::registerObserver(shared_ptr<View> observer) { observers.push_back(observer); }

void Board::init() {
    // init server ports, otherwise all cells default empty
    grid.at(0).at(3).isServerPort = true;
    grid.at(0).at(3).serverPortOwner = 0;

    grid.at(0).at(4).isServerPort = true;
    grid.at(0).at(4).serverPortOwner = 0;

    grid.at(7).at(3).isServerPort = true;
    grid.at(7).at(3).serverPortOwner = 1;

    grid.at(7).at(4).isServerPort = true;
    grid.at(7).at(4).serverPortOwner = 1;

    notifyObservers(0, 3);
    notifyObservers(0, 4);
    notifyObservers(7, 3);
    notifyObservers(7, 4);
}

void Board::notifyObservers(int r, int c) {
    View::CellState state;
    const Cell& cell = getCell(r, c);
    if (!cell.links.empty()) {
        state.link = cell.links.front()->getSymbol();
        state.linkOwnerId = cell.links.front()->getOwner();
        state.linkType = cell.links.front()->getType();
        state.isRevealed = cell.links.front()->getIsRevealed();
    } else {
        state.link = '.';
    }
    state.hasFirewall = cell.hasFirewall;
    state.isServerPort = cell.isServerPort;
    state.firewallOwnerId = cell.firewallOwner;
    state.serverPortOwnerId = cell.serverPortOwner;

    for (shared_ptr<View> observer : observers) {
        observer->notify(r, c, state);
    }
}

void Board::initLink(int r, int c, shared_ptr<Link> link) {
    if (!isInBounds(r, c)) {
        cerr << "Error: board can't init link - out of bounds.\n";
        return;
    }

    Cell& targetCell = grid[r][c];
    if (targetCell.links.size() > 0) {
        cerr << "Error: target cell already occupied by a link.\n";
        return;
    }

    targetCell.links.emplace_back(link);
    // notify observers
    notifyObservers(r, c);
}

Board::MoveResult Board::moveLink(int r, int c, shared_ptr<Link> link) {
    MoveResult move;
    move.newR = r;
    move.newC = c;
    move.symbol = link->getSymbol();

    if (!isInBounds(r, c)) {
        cout << "Error: board can't move link - out of bounds.\n";
        return move;
    }

    Cell& targetCell = grid[r][c];

    // link must be on the board to move it!
    pair<int, int> loc = getLinkLocation(link);
    int oldR = loc.first;
    int oldC = loc.second;
    if (oldR == -1 || oldC == -1) {
        cout << "Error: link not found on the board.\n";
        return move;
    }

    // target cell contains a link already
    if (!targetCell.links.empty()) {
        if (link->getOwner() == targetCell.links.front()->getOwner()) {
            cout << "Error: target cell occupied by your own link\n";
            return move;
        }
    }

    // cant move onto your own server port
    if (targetCell.isServerPort && targetCell.serverPortOwner == link->getOwner()) {
        cout << "Error: can't move onto your own server port\n";
        return move;
    }

    grid[oldR][oldC].links.clear();
    move.oldR = oldR;
    move.oldC = oldC;

    // update target cell properties
    targetCell.links.emplace_back(link);
    // notify observers
    notifyObservers(r, c);
    notifyObservers(oldR, oldC); // clear old link

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

bool Board::placeFirewall(int r, int c, int ownerId) {
    if (!isInBounds(r, c)) {
        cerr << "Error: can't place firewall out of bounds\n";
        return false;
    }

    Cell& cell = grid[r][c];
    if (cell.hasFirewall) {
        cout << "Error: cell already has a firewall\n";
        return false;
    } else if (cell.isServerPort) {
        cout << "Error: can't place firewall on a server port\n";
        return false;
    }

    cell.hasFirewall = true;
    cell.firewallOwner = ownerId;

    notifyObservers(r, c);

    return true;
}

void Board::removeLink(shared_ptr<Link> link) {
    // find the link on the board
    pair<int, int> loc = getLinkLocation(link);
    int r = loc.first;
    int c = loc.second;
    if (isInBounds(r, c)) {
        // find the link in the cell and remove it
        for (auto it = grid[r][c].links.begin(); it != grid[r][c].links.end(); ++it) {
            if (*it == link) {
                grid[r][c].links.erase(it);
                break;
            }
        }
        notifyObservers(r, c);
    } else {
        cout << "Error: can't remove link - out of bounds.\n";
    }
}
