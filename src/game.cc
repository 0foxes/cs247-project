#include "game.h"
#include "graphicdisplay.h"

// assume 2 players. Initializes 2 players, and an empty 8x8 board.
// inputs needed: link locations per player, abilities list per player
Game::Game(string link1, string link2, string ability1, string ability2, bool isGraphic)
    : board{Board()}, players{Player(1, 'a'), Player(2, 'A')}, currPlayerTurn{0} {
    // register text and graphic displays
    this->registerObserver(make_shared<TextDisplay>());
    if (isGraphic) {
        this->registerObserver(make_shared<GraphicDisplay>(8));
    }

    board.init();
    players[0].init(link1, ability1);
    players[1].init(link2, ability2);

    for (auto link : players[0].getOwnedLinks()) {
        // notify(0 + (link.first == 3 || link.first == 4), link.first,
        //        link.first + players[0].base());
        board.moveLink(0 + (link.first == 3 || link.first == 4), link.first, link.second);
    }
    for (auto link : players[1].getOwnedLinks()) {
        // notify(7 - (link.first == 3 || link.first == 4), link.first,
        //        link.first + players[1].base());
        board.moveLink(7 - (link.first == 3 || link.first == 4), link.first, link.second);
    }
}

void Game::registerObserver(shared_ptr<View> observer) {
    observers.push_back(observer);
    board.registerObserver(observer);
    players[0].registerObserver(observer);
    players[1].registerObserver(observer);
}

// void Game::printGame(ostream& out) {
//     for (shared_ptr<View> observer : observers) {
//         observer->print(out);
//     }
// }

void Game::printGame(ostream& out) {
    out << endl;
    out << "Next turn: " << players[currPlayerTurn].getName() << endl;
    out << "--------" << endl;
    players[0].print(out);
    out << "========" << endl;
    board.print(out);
    out << "========" << endl;
    players[1].print(out);
    out << endl;
}

void Game::printAbilities(ostream& out) { players[currPlayerTurn].printabilities(out); }

void Game::notify(int r, int c, char state) {
    for (shared_ptr<View> observer : observers) {
        observer->notify(r, c, state);
    }
}

void Game::moveLink(char link, char direction) {
    // check that the player does in fact own the specified link
    int index = link - players[currPlayerTurn].getBaseSymbol();
    shared_ptr<Link> movedlink;

    if (players[currPlayerTurn].owns(link)) {
        movedlink = players[currPlayerTurn].getOwnedLinks().at(index);
    } else {
        cerr << "current player cannot move the specified link";
    }
    board.moveLink(movedlink, direction);
}

void Game::endTurn() {
    currPlayerTurn++;
    currPlayerTurn %= 2;
}

bool Game::useAbility(int playerId, int abilityId, istream& in) {
    // validate args
    if (playerId > players.size()) {
        cerr << "Error: invalid player id " << playerId << endl;
        return false;
    }

    Player& player = players.at(playerId);

    // use the ability
    if (!player.useAbility(abilityId, in, *this)) {
        cerr << "Error: failed to use ability " << abilityId << endl;
        return false;
    }

    return true;
}
