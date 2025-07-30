#include "../includes/polarize_ability.h"
#include "../includes/game.h"
#include "../includes/player.h"
#include <memory>

using namespace std;

PolarizeAbility::PolarizeAbility(Player* owner) : Ability(owner) {}

bool PolarizeAbility::use(Game& game, istream& in) {
    char link;

    // args are row, col
    if (!(in >> link)) {
        cerr << "Error: polarize arg missing\n";
        return false;
    }

    // polarize the link
    try {
        int index = link - game.getNextPlayer().getBaseSymbol();
        shared_ptr<Link> polarizedLink = game.getNextPlayer().getOwnedLinks().at(index);
        polarizedLink->setType(polarizedLink->getType() == LinkType::DATA ? LinkType::VIRUS
                                                                          : LinkType::DATA);
    } catch (const out_of_range& e) {
        int index = link - game.getCurrentPlayer().getBaseSymbol();
        shared_ptr<Link> polarizedLink = game.getCurrentPlayer().getOwnedLinks().at(index);
        polarizedLink->setType(polarizedLink->getType() == LinkType::DATA ? LinkType::VIRUS
                                                                          : LinkType::DATA);
    }

    // inc uses of this ability
    uses++;
    return true;
}

string PolarizeAbility::getName() const { return "Polarize"; }
