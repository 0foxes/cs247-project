#include "../includes/expatriation_ability.h"
#include "../includes/game.h"
#include "../includes/player.h"
#include <memory>

using namespace std;

ExpatriationAbility::ExpatriationAbility(Player* owner) : Ability(owner) {}

bool ExpatriationAbility::use(Game& game, istream& in) {
    char link;
    int r, c;

    // args are row, col
    if (!(in >> link >> r >> c)) {
        cerr << "Error: expatriation arg missing\n";
        return false;
    }

    // move the link
    int index = link - owner->getBaseSymbol();
    shared_ptr<Link> movedlink;

    if (owner->owns(link)) {
        movedlink = owner->getOwnedLinks().at(index);
    } else {
        cerr << "current player cannot move the specified link";
        return false;
    }
    game.getBoard().moveLink(r, c, movedlink);

    // inc uses of this ability
    uses++;
    return true;
}

string ExpatriationAbility::getName() const { return "Expatriation"; }
