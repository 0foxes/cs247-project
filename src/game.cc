#include "game.h"

// assume 2 players. Initializes 2 players, and an empty 8x8 board.
// inputs needed: link locations per player, abilities list per player
Game::Game(std::string link1, std::string link2, bool isGraphic)
    : board{Board()}, players{Player(1, 'a'), Player(2, 'A')},
      currPlayerTurn{0} {

    if (!isGraphic)
        this->registerObserver(std::make_shared<TextDisplay>());

    board.init();
    players[0].init(link1);
    players[1].init(link2);

    for (auto link : players[0].ownedLinks()) {
        // notify(0 + (link.first == 3 || link.first == 4), link.first,
        //        link.first + players[0].base());
        board.moveLink(0 + (link.first == 3 || link.first == 4), link.first,
                       link.second);
    }
    for (auto link : players[1].ownedLinks()) {
        // notify(7 - (link.first == 3 || link.first == 4), link.first,
        //        link.first + players[1].base());
        board.moveLink(7 - (link.first == 3 || link.first == 4), link.first,
                       link.second);
    }
}

void Game::registerObserver(std::shared_ptr<View> observer) {
    observers.push_back(observer);
    board.registerObserver(observer);
    players[0].registerObserver(observer);
    players[1].registerObserver(observer);
}

// void Game::printGame(std::ostream& out) {
//     for (std::shared_ptr<View> observer : observers) {
//         observer->print(out);
//     }
// }

void Game::printGame(std::ostream& out) {
    players[0].print(out);
    board.print(out);
    players[1].print(out);
}

void Game::notify(int r, int c, char state) {
    for (std::shared_ptr<View> observer : observers) {
        observer->notify(r, c, state);
    }
}

void Game::moveLink(char link, char direction) {
    // check that the player does in fact own the specified link
    int index = link - players[currPlayerTurn].base();
    std::shared_ptr<Link> movedlink;

    if (players[currPlayerTurn].owns(link)) {
        movedlink = players[currPlayerTurn].ownedLinks().at(index);
    } else {
        std::cerr << "current player cannot move the specified link";
    }
    board.moveLink(movedlink, direction);
}

void Game::endTurn() {
    currPlayerTurn++;
    currPlayerTurn %= 2;
}