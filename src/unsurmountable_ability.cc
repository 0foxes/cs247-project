#include "../includes/unsurmountable_ability.h"
#include "../includes/game.h"
#include "../includes/player.h"

using namespace std;

UnsurmountableAbility::UnsurmountableAbility(Player* owner) : Ability(owner) {}

bool UnsurmountableAbility::use(Game& game, istream& in) {
    // no args
    // unsurmountable the links
    for (auto link : owner->getOwnedLinks()) {
        link.second->setStrength(link.second->getStrength() + 10);
    }
    owner->setUnsurmountable(true);

    // inc uses of this ability
    uses++;
    return true;
}

string UnsurmountableAbility::getName() const { return "Unsurmountable"; }
