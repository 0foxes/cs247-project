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
// empty or have a link
// normal or a server port or have a firewall
// (in the latter two cases they'll never actually have an associated link
// though)
class Board {
    // die
    // vector<vector<CellState>> grid;
    // Each cell can either be: empty, a server port, a link, or ???
    // if the cell has a link it will be specified here
    // shared_ptr<Link> link;
    vector<shared_ptr<View>> observers;
    vector<vector<CellState>> grid;
    map<shared_ptr<Link>, pair<int, int>> linkLocs;
    // key{a, b} is a server port owned by player w id {value}
    map<pair<int, int>, int> serverPorts;
    // key{a, b} holds link {value}
    map<pair<int, int>, shared_ptr<Link>> occupied;

    // check if a position is in the board
    bool isInBounds(int r, int c) const;

  public:
    Board();
    void registerObserver(shared_ptr<View> observer);
    void print(ostream& out);
    // updateCell(CellState state, int r, int c);
    // moveLinkTo(Link link, int r, int c);
    void init();

    void moveLink(int r, int c, shared_ptr<Link> link);
    void moveLink(shared_ptr<Link> link,
                  char direction); // direction is definitely u, d, l, or r

    void notify(int r, int c, char state);
};

#endif