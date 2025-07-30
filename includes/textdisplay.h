#ifndef __TEXTDISPLAY_H__
#define __TEXTDISPLAY_H__
#include "view.h"
#include <iostream>
#include <map>
#include <vector>

using namespace std;

class TextDisplay : public View {
    // state things associated with a player
    typedef struct {
        vector<shared_ptr<Link>> links;
        vector<shared_ptr<Link>> downloaded;
    } PlayerState;

    vector<vector<char>> theBoard;

    // The player id this view is associated with
    // -1 if from both (neutral)
    int currPlayerId = -1;

    // state of each player by player id
    map<int, PlayerState> playerStates;

    // helper to print a player state
    void printPlayer(int id, ostream& out);

  public:
    TextDisplay();

    // What can change?
    // typically, a link probably changed states for some reason
    // 1. link on board moved
    // 2. link on board got downloaded by a player
    // 3. cell on board got turned into something (a firewall?)
    // 4. turn changed (so player pov should change)
    void notify(int r, int c, CellState cell) override;

    // update player state
    void notify(int playerId, vector<shared_ptr<Link>> links,
                vector<shared_ptr<Link>> downloads) override;

    void notifyCurrPlayer(int playerId) override { currPlayerId = playerId; }

    /*
     * Prints the grid as specified in the assignment specification.
     */
    void print(ostream& out) override;
};

#endif
