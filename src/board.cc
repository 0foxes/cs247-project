#include "../includes/board.h"

Board::Board()
    : grid{vector<vector<CellState>>(8, vector<CellState>(8, CellState::NORMAL_EMPTY))} {}

void Board::registerObserver(shared_ptr<View> observer) { observers.push_back(observer); }

void Board::init() {
    grid.at(0).at(3) = CellState::SERVER_PORT;
    notify(0, 3, 'S');
    grid.at(0).at(4) = CellState::SERVER_PORT;
    notify(0, 4, 'S');
    grid.at(7).at(3) = CellState::SERVER_PORT;
    notify(7, 3, 'S');
    grid.at(7).at(4) = CellState::SERVER_PORT;
    notify(7, 4, 'S');
}

void Board::notify(int r, int c, char state) {
    for (shared_ptr<View> observer : observers) {
        observer->notify(r, c, state);
    }
}

void Board::print(ostream& out) {
    for (shared_ptr<View> observer : observers) {
        observer->print(out);
    }
    // for (auto i : linkLocs) {
    //     out << i.first->symbol() << "(" << i.second.first << ", "
    //         << i.second.second << ") ";
    // }
    // out << endl;
}

void Board::moveLink(int r, int c, shared_ptr<Link> link) {
    if (linkLocs.count(link) > 0 && !(occupied.count({r, c}) > 0)) {
        // if link is already in map somewhere and the cell is unpopulated
        notify(linkLocs.at(link).first, linkLocs.at(link).second, '.');
        occupied.erase(linkLocs.at(link));
        linkLocs.at(link) = {r, c};
        occupied.insert({{r, c}, link});
        notify(r, c, link->getSymbol());
    } else if (!(linkLocs.count(link) > 0) && occupied.count({r, c}) > 0 &&
               serverPorts.count({r, c}) > 0) {
        // if link is not in map and the cell is populated
        cerr << "can't insert a new link in an already-populated cell or "
                "server port\n";
    } else if (!(linkLocs.count(link) > 0) && occupied.count({r, c}) > 0) {
        // if link is in map and the cell is populated
        if (link->getOwner() == occupied.at({r, c})->getOwner()) {
            cerr << "cannot move to a cell occupied by ur own thing";
        } else if (link->getOwner() != occupied.at({r, c})->getOwner()) {
            cout << "u battled and idk what happens next??";
        }
    } else { // link does not exist, can just be inserted
        linkLocs.insert({link, {r, c}});
        occupied.insert({{r, c}, link});
        notify(r, c, link->getSymbol());
    }
    // for (shared_ptr<View> observer : observers) {
    //     observer->notify(r, c, link->symbol());
    // }
}

bool Board::isInBounds(int r, int c) const {
    // assumes grid is rectangle
    return (r >= 0) && (r < grid.size()) && (c >= 0) && (c < grid[0].size());
}

void Board::moveLink(shared_ptr<Link> link, char direction) {
    if (!(linkLocs.count(link) > 0)) {
        cerr << "link does not exist?" << endl;
        return;
    }

    // boosted links move 2 cells instead of 1
    int distance = 1;
    if (link->getIsBoosted()) {
        distance = 2;
    }

    pair<int, int> currLoc = linkLocs.at(link);
    int newRow = currLoc.first;
    int newCol = currLoc.second;

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
        cout << "invalid direction" << endl;
        return;
    }

    // move if allowed
    if (isInBounds(newRow, newCol)) {
        moveLink(newRow, newCol, link);
    } else {
        cout << "move out of bounds. did not move." << endl;
    }
}
