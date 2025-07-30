#ifndef __GRAPHICDISPLAY_H__
#define __GRAPHICDISPLAY_H__

#include "view.h"
#include "window.h"
#include <map>
#include <vector>

using namespace std;

class GraphicDisplay : public View {
    Xwindow xw;
    int cellWidth;                    // size of each cell in pixels
    int gridSize;                     // size of board, assuming grid (n x n)
    vector<vector<int>> displayState; // cached state of each cell

    // state of each player by player id
    map<int, PlayerState> playerStates;
    int currPlayerId = -1; // current player id, -1 if neutral

    // helper to draw a player state
    void drawPlayerState(int playerId);

  public:
    GraphicDisplay(int n);

    // redraw the cell for (r, c) to contain the item `change`
    void notify(int r, int c, CellState cell) override;
    void notify(int playerId, vector<shared_ptr<Link>> links, vector<shared_ptr<Link>> downloads,
                vector<shared_ptr<Ability>>) override;
    void notifyCurrPlayer(int playerId) override { currPlayerId = playerId; }
    void print(ostream& out) override;
};

#endif
