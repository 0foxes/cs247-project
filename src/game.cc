#include "../includes/game.h"
#include "../includes/graphicdisplay.h"

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

    // place links on the board in starting locations. note that links d/D and e/E must be in front
    // of server port cells
    int p0LinkCol = 0;
    for (auto link : players[0].getOwnedLinks()) {
        if (link->getSymbol() == 'd' || link->getSymbol() == 'e') {
            board.initLink(1, p0LinkCol++, link);
        } else {
            board.initLink(0, p0LinkCol++, link);
        }
    }
    int p1LinkCol = 0;
    for (auto link : players[1].getOwnedLinks()) {
        if (link->getSymbol() == 'D' || link->getSymbol() == 'E') {
            board.initLink(6, p1LinkCol++, link);
        } else {
            board.initLink(7, p1LinkCol++, link);
        }
    }

    // notify observers
    for (shared_ptr<View> observer : observers) {
        observer->notify(1, players[0].getOwnedLinks(), players[0].getDownloaded());
        observer->notify(2, players[1].getOwnedLinks(), players[1].getDownloaded());
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
    for (shared_ptr<View> observer : observers) {
        observer->print(out);
    }
    // out << endl;
    // out << "Next turn: " << players[currPlayerTurn].getName() << endl;
    // out << "--------" << endl;
    // // players[0].print(out);
    // out << "========" << endl;
    // board.print(out);
    // out << "========" << endl;
    // // players[1].print(out);
    // out << endl;
}

void Game::printAbilities(ostream& out) { players[currPlayerTurn].printabilities(out); }

void Game::notify(int r, int c, char state) {
    for (shared_ptr<View> observer : observers) {
        observer->notify(r, c, state);
    }
}

bool Game::moveLink(char link, char direction) {
    // check that the player does in fact own the specified link
    int index = link - players[currPlayerTurn].getBaseSymbol();
    shared_ptr<Link> movedlink;

    if (players[currPlayerTurn].owns(link)) {
        movedlink = players[currPlayerTurn].getOwnedLinks().at(index);
    } else {
        cerr << "current player cannot move the specified link";
        return false;
    }
    Board::MoveResult move = board.moveLink(movedlink, direction);
    if (move.newR == -1 || move.newC == -1) {
        cerr << "Error: link cant be moved to the specified location.\n";
        return false;
    }

    for (shared_ptr<View> observer : observers) {
        observer->notify(move.oldR, move.oldC, '.');         // clear old link
        observer->notify(move.newR, move.newC, move.symbol); // new link
    }

    return true;
}

void Game::endTurn() {
    currPlayerTurn++;
    currPlayerTurn %= 2;
    if (players[currPlayerTurn].getUnsurmountable()) {
        players[currPlayerTurn].setUnsurmountable(false);
        for (auto link : players[currPlayerTurn].getOwnedLinks()) {
            link->setStrength(link->getStrength() - 10);
        }
    }
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
