#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "link.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class View {
  public:
    /*
     * The Controller calls notify to
     * update the (r,c) location to contain item
     */
    virtual void notify(int r, int c, char item) = 0;

    /**
     * Notify the view of a player state change.
     * should be called on change of player's links, downloads, etc
     */
    virtual void notify(int playerId, vector<shared_ptr<Link>> links,
                        vector<shared_ptr<Link>> downloads) = 0;

    /**
     * who's turn it is
     */
    virtual void notifyCurrPlayer(int playerId) = 0;

    virtual void print(ostream& out) = 0;

    virtual ~View() = 0;
};

#endif
