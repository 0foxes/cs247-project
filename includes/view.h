#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "ability.h"
#include "link.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class View {
  public:
    // state of a cell from the view's pov
    typedef struct {
        char link = '.'; // link existing in this cell or '.' if no link
        bool hasFirewall = false;
        bool isServerPort = false;
        // owner ids. -1 if not relevant
        int linkOwnerId = -1;
        int firewallOwnerId = -1;
        int serverPortOwnerId = -1;
    } CellState;

    // state things associated with a player
    typedef struct {
        vector<shared_ptr<Link>> links;
        vector<shared_ptr<Link>> downloaded;
        vector<shared_ptr<Ability>> abilities;
    } PlayerState;

    /*
     * The Controller calls notify to
     * update the (r,c) location to contain item
     */
    virtual void notify(int r, int c, CellState cell) = 0;

    /**
     * Notify the view of a player state change.
     * should be called on change of player's links, downloads, etc
     */
    virtual void notify(int playerId, vector<shared_ptr<Link>> links,
                        vector<shared_ptr<Link>> downloads, vector<shared_ptr<Ability>>) = 0;

    /**
     * who's turn it is
     */
    virtual void notifyCurrPlayer(int playerId) = 0;

    virtual void print(ostream& out) = 0;

    virtual ~View() = 0;
};

#endif
