#ifndef __GAME_H__
#define __GAME_H__

#include "board.h"
#include "error.h"
#include "player.h"
#include "textdisplay.h"
#include "view.h"
#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class Game {
    static constexpr int NUM_PLAYERS = 2;

    // The board
    // unique_ptr<Board> theBoard;
    Board board;
    // Associated players
    vector<Player> players;
    // all associated links
    vector<shared_ptr<Link>> allLinks;
    // vector<Link> links;
    // Observers (or views)
    vector<shared_ptr<View>> observers;
    int currPlayerTurn; // Index of the current player whose turn it is

    /*
     * The following field helps track how the game has progressed.
     * The field should track how many Cells of the grid are of each color.
     * Whenever a Cell changes color, the corresponding color count should
     * be updated.
     */
    // unsigned int colours[5];
    // GameNotification* notification; // object registered for notifications
    // Add private members, if necessary.

  public:
    Game(string player1links, string player2links, string player1abilities, string player2abilities,
         bool isGraphic); // assume 2 players. Initializes 2 players,
                          // and an empty 8x8 board.
    void moveLink(char link, char direction);
    void registerObserver(shared_ptr<View> observer);

    // ask the game to print the views (for text)
    void printGame(ostream& out);
    void printAbilities(ostream& out);

    /*
     * Receive update notification from Cells.
     */
    // void notify(int r, int c, int oldState, int newState);

    /*
     * The following method is used to determine if the game has been won.
     * ???
     */
    // bool isWon();
    // void
    // init(int n,
    //  GameNotification* gameNotification); // Sets up an n x n grid.  Clears
    // old grid, if necessary.
    // void change(int c); // Changes (0,0) and all appropriate neighbours to c
    // void init(int r, int c,
    //           int change); // Called by Controller:init in response to
    // initialization requests when game is in init mode
    void notify(int r, int c, char state);
    void endTurn();

    Board& getBoard() { return board; }
    Player& getCurrentPlayer() { return players[currPlayerTurn]; }
    Player& getNextPlayer() { return players[1 - currPlayerTurn]; }
    int getCurrPlayerId() { return currPlayerTurn; }

    // use an ability for player. `in` is the ability args
    bool useAbility(int playerId, int ablityId, istream& in);
};

#endif
