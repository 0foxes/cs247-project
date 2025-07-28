#ifndef __TEXTDISPLAY_H__
#define __TEXTDISPLAY_H__
#include "view.h"
#include <iostream>
#include <vector>

using namespace std;

class TextDisplay : public View {
    vector<vector<char>> theBoard;
    // The player id this view is associated with
    // -1 if from both
    int playerId;

  public:
    TextDisplay();

    // What can change?
    // typically, a link probably changed states for some reason
    // 1. link on board moved
    // 2. link on board got downloaded by a player
    // 3. cell on board got turned into something (a firewall?)
    // 4. turn changed (so player pov should change)
    void notify(int r, int c, char change) override;

    /*
     * Prints the grid as specified in the assignment specification.
     */
    void print(ostream& out) override;
};

#endif
