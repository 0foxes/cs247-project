#include "../includes/scan_ability.h"
#include "../includes/game.h"
#include "../includes/player.h"
#include <memory>

using namespace std;

ScanAbility::ScanAbility(Player* owner) : Ability(owner) {}

bool ScanAbility::use(Game& game, istream& in) {
    char link;

    // args are row, col
    if (!(in >> link)) {
        cerr << "Error: scan arg missing\n";
        return false;
    }

    // scan the link
    int index = link - game.getNextPlayer().getBaseSymbol();
    shared_ptr<Link> scannedLink;
    if (game.getNextPlayer().owns(link)) {
        scannedLink = game.getNextPlayer().getOwnedLinks().at(index);
        cout << "type " << (scannedLink->getType() == 1 ? "DATA" : "VIRUS") << " strength " << scannedLink->getStrength() << endl;
    } else {
        cerr << "current player cannot scan own link" << endl;
        return false;
    }

    // inc uses of this ability
    uses++;
    return true;
}

string ScanAbility::getName() const { return "Scan"; }
