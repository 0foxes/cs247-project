#include "../includes/game.h"
#include "../includes/graphicdisplay.h"
#include "../includes/view.h"

// assume 2 players. Initializes 2 players, and an empty 8x8 board.
// inputs needed: link locations per player, abilities list per player
Game::Game(string link1, string link2, string ability1, string ability2, bool isGraphic)
    : board{Board()}, players{Player(1, 'a'), Player(2, 'A')}, currPlayerTurn{0} {
    // register text and graphic displays
    this->registerObserver(make_shared<TextDisplay>());
    if (isGraphic) {
        this->registerObserver(make_shared<GraphicDisplay>(8));
    }

    // must notify who's turn it is before initializing everything
    for (shared_ptr<View> observer : observers) {
        observer->notifyCurrPlayer(1); // start with player 1
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
}

void Game::registerObserver(shared_ptr<View> observer) {
    observers.push_back(observer);
    board.registerObserver(observer);
    players[0].registerObserver(observer);
    players[1].registerObserver(observer);
}

void Game::printGame(ostream& out) {
    for (shared_ptr<View> observer : observers) {
        observer->print(out);
    }
}

void Game::printAbilities(ostream& out) { players[currPlayerTurn].printabilities(out); }

/**
 * - physically moving the link on the board (board.movelink)
 * - do firewall logic
 * - do serverport logic
 * - do battle logic
 */
bool Game::moveLink(char link, char direction) {
    // opponent id
    int opponentId = (currPlayerTurn + 1 == 1) ? 2 : 1;

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

    // firewall logic
    // check if link moved into opponents firewall
    int firewallOwner = board.getFirewallOwner(move.newR, move.newC);
    if (firewallOwner == opponentId) {
        // reveal the link
        movedlink->setRevealed(true);
        // if the link is a virus, download it to its owner
        if (movedlink->getType() == LinkType::VIRUS) {
            players[firewallOwner - 1].download(movedlink);
            board.removeLink(movedlink);
        }
    }

    // server port logic
    // check if link moved into opponents server port
    int serverportOwner = board.getServerPortOwner(move.newR, move.newC);
    if (serverportOwner == opponentId) {
        players[opponentId - 1].download(movedlink);
        board.removeLink(movedlink);
    }

    // battle!!!!!!
    // check if link moved into opponents link
    shared_ptr<Link> existingLink = board.getLink(move.newR, move.newC);
    if (existingLink != nullptr && existingLink->getOwner() == opponentId) {
        // reveal both links
        movedlink->setRevealed(true);
        existingLink->setRevealed(true);

        // winner is player whos link has higher strength.
        // winner downloads the link of the loser
        int winnerId = -1;
        if (movedlink->getStrength() > existingLink->getStrength()) {
            winnerId = currPlayerTurn + 1; // player ids are 1-indexed
            players[currPlayerTurn].download(existingLink);
            board.removeLink(existingLink);
        } else if (movedlink->getStrength() < existingLink->getStrength()) {
            winnerId = opponentId;
            players[opponentId - 1].download(movedlink);
            board.removeLink(movedlink);
        } else {
            // equal strenght - initiater wins
            winnerId = currPlayerTurn + 1;
            players[currPlayerTurn].download(existingLink);
            board.removeLink(existingLink);
        }
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board.notifyObservers(i, j);
        }
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

    // reset abilities used count
    abilitiesUsed = 0;

    // update current player id in observers
    for (shared_ptr<View> observer : observers) {
        observer->notifyCurrPlayer(currPlayerTurn + 1); // +1 since player ids are 1-indexed
    }
}

bool Game::useAbility(int playerId, int abilityId, istream& in) {
    // can only use 1 ability per turn
    if (abilitiesUsed > 0) {
        cout << "Error: can only use 1 ability per turn\n";
        return false;
    }

    // validate args
    if (playerId > players.size()) {
        cerr << "Error: invalid player id " << playerId << endl;
        return false;
    }

    Player& player = players.at(playerId);

    // use the ability
    if (player.useAbility(abilityId, in, *this)) {
        abilitiesUsed++;
    } else {
        cerr << "Error: failed to use ability " << abilityId << endl;
        return false;
    }

    return true;
}
