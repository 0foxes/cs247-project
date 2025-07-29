#include "../includes/game.h"
#include "../includes/link_boost_ability.h"
#include "../includes/player.h"

using namespace std;

LinkBoostAbility::LinkBoostAbility(Player* owner) : Ability(owner) {}

bool LinkBoostAbility::use(Game& game, istream& in) {
    char linkId;

    // first arge is link id to boost
    if (!(in >> linkId)) {
        cerr << "Error: link boost arg missing\n";
        return false;
    }

    // make sure the link belongs to the player using the ability
    Player* user = getOwner();
    if (!user) {
        cerr << "Error: owner is null\n";
        return false;
    }

    // boost the link
    std::shared_ptr<Link> link = user->getLink(linkId);
    if (!link) {
        cerr << "Error: invalid link ID\n";
        return false;
    }
    link->setBoosted(true);

    // inc uses of this ability
    uses++;

    return true;
}

string LinkBoostAbility::getName() const { return "Link Boost"; }
