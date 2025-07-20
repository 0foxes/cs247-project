#ifndef __BOARD_H__
#define __BOARD_H__

// Standard headers
#include <map>
#include <memory>
#include <vector>
// Local includes
#include "link.h"
#include "view.h"

enum CellState { NORMAL_EMPTY, HAS_LINK, SERVER_PORT, FIREWALL };
// A cell on the board can be
// empty or have a link
// normal or a server port or have a firewall
// (in the latter two cases they'll never actually have an associated link
// though)
class Board {
    // die
    // std::vector<std::vector<CellState>> grid;
    // Each cell can either be: empty, a server port, a link, or ???
    // if the cell has a link it will be specified here
    // std::shared_ptr<Link> link;
    std::vector<std::shared_ptr<View>> observers;
    std::vector<std::vector<CellState>> grid;
    std::map<std::shared_ptr<Link>, std::pair<int, int>> linkLocs;
    // key{a, b} is a server port owned by player w id {value}
    std::map<std::pair<int, int>, int> serverPorts;
    // key{a, b} holds link {value}
    std::map<std::pair<int, int>, std::shared_ptr<Link>> occupied;

  public:
    Board();
    void registerObserver(std::shared_ptr<View> observer);
    void print(std::ostream& out);
    // updateCell(CellState state, int r, int c);
    // moveLinkTo(Link link, int r, int c);
    void init();
    // overloading?
    void moveLink(int r, int c, std::shared_ptr<Link> link);
    void moveLink(std::shared_ptr<Link> link,
                  char direction); // direction is definitely u, d, l, or r
    void notify(int r, int c, char state);
};

#endif