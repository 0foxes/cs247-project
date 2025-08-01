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
    Board board;
    // Associated players
    vector<Player> players;
    // all associated links
    vector<shared_ptr<Link>> allLinks;
    // Observers (or views)
    vector<shared_ptr<View>> observers;
    int currPlayerTurn;    // Index of the current player whose turn it is
    int abilitiesUsed = 0; // track how many abilities used this turn

  public:
    Game(string player1links, string player2links, string player1abilities, string player2abilities,
         bool isGraphic); // assume 2 players. Initializes 2 players,
                          // and an empty 8x8 board.
    bool moveLink(char link, char direction);
    void registerObserver(shared_ptr<View> observer);

    // ask the game to print the views (for text)
    void printGame(ostream& out);
    void printAbilities(ostream& out);

    void endTurn();

    Board& getBoard() { return board; }
    // return ids, 0-indexed
    Player& getCurrentPlayer() { return players[currPlayerTurn]; }
    Player& getNextPlayer() { return players[(currPlayerTurn + 1) % 2]; }
    int getCurrPlayerId() { return currPlayerTurn; }
    int getNextPlayerId() { return (currPlayerTurn + 1) % 2; }
    int getAbilitiesUsed() const { return abilitiesUsed; }

    // use an ability for player. `in` is the ability args
    bool useAbility(int playerId, int ablityId, istream& in);

    // check if a player has won or lost, which ends the game
    bool isGameOver();
};

#endif
