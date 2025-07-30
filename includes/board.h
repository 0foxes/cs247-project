#ifndef __BOARD_H__
#define __BOARD_H__

// Standard headers
#include <map>
#include <memory>
#include <vector>
// Local includes
#include "link.h"
#include "view.h"

using namespace std;

enum CellState { NORMAL_EMPTY, HAS_LINK, SERVER_PORT, FIREWALL };

// A cell on the board can be
// - empty or have a link
// - normal or a server port or have a firewall
// (in the latter two cases they'll never actually have an associated link
// though)
class Board {
    // a cell and its properties
    struct Cell {
        shared_ptr<Link> link = nullptr; // null if no link is in this cell
        bool hasFirewall = false;
        bool isServerPort = false;
        int serverPortOwner = -1; // player id who owns this server port. -1 if no server port
        int firewallOwner = -1;   // player id who owns this firewall. -1 if no firewall
    };

    // the board of cells. assume board is rectangular grid
    vector<vector<Cell>> grid;
    // map<shared_ptr<Link>, pair<int, int>> linkLocs;
    // key{a, b} is a server port owned by player w id {value}
    // map<pair<int, int>, int> serverPorts;
    // key{a, b} holds link {value}
    // map<pair<int, int>, shared_ptr<Link>> occupied;

    vector<shared_ptr<View>> observers;

    // check if a position is in the board
    bool isInBounds(int r, int c) const;

    // helper to get the cell at (r, c)
    const Cell& getCell(int r, int c) const { return grid.at(r).at(c); }

    // helper to get the link's location on the board or -1,-1 if not found
    pair<int, int> getLinkLocation(shared_ptr<Link> link) const {
        for (int r = 0; r < grid.size(); r++) {
            for (int c = 0; c < grid[r].size(); c++) {
                if (grid[r][c].link == link) {
                    return {r, c};
                }
            }
        }
        return {-1, -1}; // not found
    }

    // helper to notify observers of a cell change
    void notifyObservers(int r, int c, char state);

  public:
    // piece `symbol` moved from `oldR`, `oldC` to `newR`, `newC`
    struct MoveResult {
        int oldR, oldC;
        int newR, newC;
        char symbol;
    };

    Board();
    void registerObserver(shared_ptr<View> observer);
    // void print(ostream& out);
    // updateCell(CellState state, int r, int c);
    // moveLinkTo(Link link, int r, int c);
    void init();

    // spawn a link into the board at (r, c)
    void initLink(int r, int c, shared_ptr<Link> link);

    // move a link already on the board to (r, c)
    MoveResult moveLink(int r, int c, shared_ptr<Link> link);

    MoveResult moveLink(shared_ptr<Link> link, char direction); // direction must be u, d, l, or r
};

#endif