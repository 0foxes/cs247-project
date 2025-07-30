#include "textdisplay.h"
#include "board.h"

TextDisplay::TextDisplay() : theBoard{vector<vector<char>>(8, vector<char>(8, '.'))} {}

/*
 * updates the symbol at position r, c
 */
void TextDisplay::notify(int r, int c, CellState cell) {
    char itemSymbol = cell.link;

    // prioritize displaying link, then firewall and serverport
    if (itemSymbol != '.') {
        itemSymbol = cell.link;
    } else if (cell.hasFirewall) {
        // firewall symbol upsidedown for player 1
        itemSymbol = (cell.firewallOwnerId == 1) ? 'm' : 'w';
    } else if (cell.isServerPort) {
        itemSymbol = 'S';
    }
    theBoard[r][c] = itemSymbol;
}

void TextDisplay::notify(int playerId, vector<shared_ptr<Link>> links,
                         vector<shared_ptr<Link>> downloads,
                         vector<shared_ptr<Ability>> abilities) {
    PlayerState& ps = playerStates[playerId];

    // update player state
    ps.links = links;
    ps.downloaded = downloads;
    ps.abilities = abilities;
}

void TextDisplay::printPlayer(int id, ostream& out) {
    int viruses = 0;
    int datas = 0;
    PlayerState& p = playerStates.at(id);

    out << "Player " << id << ":" << endl;

    for (auto link : p.downloaded) {
        viruses += link->getType() == LinkType::VIRUS;
        datas += link->getType() == LinkType::DATA;
    }

    // abilities
    int abilitiesAvail = 0;
    for (auto& ability : p.abilities) {
        if (!ability->isUsed()) {
            abilitiesAvail++;
        }
    }
    out << "Abilities: " << abilitiesAvail << endl;

    // downloads
    out << "Downloaded: " << to_string(datas) << "D, " << to_string(viruses) << "V" << endl;

    bool isfirst = true;
    for (auto link : p.links) {
        if (!isfirst) {
            out << " ";
        }
        isfirst = false;
        // only print opponent links if they are revealed or its the current player's
        if (link->getIsRevealed() || id == currPlayerId) {
            out << link->getSymbol() << ": " << *link;
        } else {
            out << link->getSymbol() << ": ?";
        }
    }
    out << endl;
}

/*
 * Prints the grid as specified in the assignment specification.
 */
void TextDisplay::print(ostream& out) {
    // current player
    out << "Current Player: " << currPlayerId << endl;
    out << "--------\n";

    // player1
    printPlayer(1, out);

    // the board
    out << "========\n";
    for (vector<char> row : theBoard) {
        for (char element : row) {
            out << element;
        }
        out << "\n";
    }
    out << "========\n";

    // player 2
    printPlayer(2, out);

    out << endl;
}
