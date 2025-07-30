#include "../includes/firewall_ability.h"
#include "../includes/board.h"
#include "../includes/game.h"
#include "../includes/player.h"

using namespace std;

FirewallAbility::FirewallAbility(Player* owner) : Ability(owner) {}

bool FirewallAbility::use(Game& game, istream& in) {
    int r, c;
    if (!(in >> r >> c)) {
        cerr << "Error: firewall args missing\n";
        return false;
    }

    // check ownership
    if (!getOwner()) {
        cerr << "Error: owner is null\n";
        return false;
    }

    if (!game.getBoard().isInBoard(r, c)) {
        cerr << "Error: cant place firewall out of bounds\n";
        return false;
    }

    // set cell as firewall
    game.getBoard().placeFirewall(r, c, getOwner()->getId());

    // inc uses of this ability
    uses++;

    return true;
}

string FirewallAbility::getName() const { return "Firewall"; }
