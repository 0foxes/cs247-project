#include "../includes/stork_visitation_ability.h"
#include "../includes/game.h"
#include "../includes/player.h"
#include <memory>

using namespace std;

StorkVisitationAbility::StorkVisitationAbility(Player* owner) : Ability(owner) {}

bool StorkVisitationAbility::use(Game& game, istream& in) {
    int r, c;

    // args are row, col
    if (!(in >> r >> c)) {
        cerr << "Error: stork visitation arg missing\n";
        return false;
    }

    // spawn new link
    shared_ptr<Link> newlink = make_shared<Link>();
    newlink->setOwner(owner->getId());
    if (rand() % 2) {
        newlink->makeData();
    } else {
        newlink->makeVirus();
    }
    newlink->setStrength(1); // baby links are weak
    newlink->setSymbol(owner->getBaseSymbol() + owner->getOwnedLinks().size() +
                       uses); // should be new symbol
    owner->addLink(owner->getOwnedLinks().size() + uses, newlink);
    game.getBoard().moveLink(r, c, newlink);

    // inc uses of this ability
    uses++;

    return true;
}

string StorkVisitationAbility::getName() const { return "Stork Visitation"; }
