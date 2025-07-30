#include "textdisplay.h"
#include "board.h"

TextDisplay::TextDisplay() : theBoard{vector<vector<char>>(8, vector<char>(8, '.'))} {}

/*
 * updates the symbol at position r, c
 */
// for now there is no way to update player state since its not a part of the
// display rn
void TextDisplay::notify(int r, int c, char change) { theBoard.at(r).at(c) = change; }

void TextDisplay::notify(int playerId, vector<shared_ptr<Link>> links,
                         vector<shared_ptr<Link>> downloads) {
    PlayerState& ps = playerStates[playerId];

    // update player state
    ps.links = links;
    ps.downloaded = downloads;
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

    out << "Downloaded: " << to_string(datas) << "D, " << to_string(viruses) << "V" << endl;

    bool isfirst = true;
    for (auto link : p.links) {
        if (!isfirst) {
            out << " ";
        }
        isfirst = false;
        out << link->getSymbol() << ": " << *link;
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
