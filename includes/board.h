#ifndef __BOARD_H__
#define __BOARD_H__

// Standard headers
#include <algorithm>
#include <map>
#include <memory>
#include <vector>
// Local includes
#include "link.h"
#include "view.h"

using namespace std;

// A cell on the board can be
// - empty or have a link
// - normal or a server port or have a firewall
// (in the latter two cases they'll never actually have an associated link
// though)
class Board {
    // a cell and its properties
    struct Cell {
        // allow for cells to have multiple links occupying (battle, or future extensions)
        vector<shared_ptr<Link>> links = {}; // empty if no link is in this cell
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

    // helper to get the cell at (r, c)
    const Cell& getCell(int r, int c) const { return grid.at(r).at(c); }

    // helper to get the link's location on the board or -1,-1 if not found
    pair<int, int> getLinkLocation(shared_ptr<Link> link) const {
        // loop all rows and cols of grid
        for (int r = 0; r < grid.size(); r++) {
            for (int c = 0; c < grid[r].size(); c++) {
                // check all links in this cell
                if (find(grid[r][c].links.begin(), grid[r][c].links.end(), link) !=
                    grid[r][c].links.end()) {
                    return {r, c};
                }
            }
        }
        return {-1, -1}; // not found
    }

    // helper to notify observers of a cell state change

  public:
    void notifyObservers(int r, int c);
    // piece `symbol` moved from `oldR`, `oldC` to `newR`, `newC`
    struct MoveResult {
        int oldR, oldC;
        int newR, newC;
        char symbol;
    };

    Board();
    void registerObserver(shared_ptr<View> observer);
    void init();

    // spawn a link into the board at (r, c)
    void initLink(int r, int c, shared_ptr<Link> link);

    // move a link already on the board to (r, c)
    MoveResult moveLink(int r, int c, shared_ptr<Link> link);

    // move a link in the specified direction.
    // returns -1 -1 -1 -1 if error.
    // returns oldR, oldC, newR, newC if successful normal move
    // returns oldR, oldC, -1, -1 if moved into a valid download edge
    MoveResult moveLink(shared_ptr<Link> link, char direction); // direction must be u, d, l, or r

    // check if a position is in the board
    bool isInBoard(int r, int c) const;

    bool placeFirewall(int r, int c, int ownerId);

    // returns firewall owner id if there is a firewall at (r, c), -1 otherwise
    int getFirewallOwner(int r, int c) const {
        return getCell(r, c).hasFirewall ? getCell(r, c).firewallOwner : -1;
    }

    // returns server port owner id if there is a server port at (r, c), -1 otherwise
    int getServerPortOwner(int r, int c) const {
        return getCell(r, c).isServerPort ? getCell(r, c).serverPortOwner : -1;
    }

    // return link on (r, c) or nullptr if no link there
    vector<shared_ptr<Link>> getLink(int r, int c) const { return getCell(r, c).links; }

    void removeLink(shared_ptr<Link> link);
};

#endif