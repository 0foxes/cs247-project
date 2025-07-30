#include "../includes/link_boost_ability.h"
#include "../includes/game.h"
#include "../includes/player.h"

using namespace std;

LinkBoostAbility::LinkBoostAbility(Player* owner) : Ability(owner) {}

bool LinkBoostAbility::use(Game& game, istream& in) {
    char linkId;
    if (!(in >> linkId)) {
        cout << "Error: link boost arg missing\n";
        return false;
    }

    // boost the link after checking ownership
    if (!getOwner()) {
        cout << "Error: owner is null\n";
        return false;
    }
    std::shared_ptr<Link> link = getOwner()->getLink(linkId);
    if (!link) {
        cout << "Error: invalid link ID\n";
        return false;
    }
    link->setBoosted(true);

    // inc uses of this ability
    uses++;

    return true;
}

string LinkBoostAbility::getName() const { return "Link Boost"; }
