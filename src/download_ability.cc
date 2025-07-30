#include "../includes/download_ability.h"
#include "../includes/game.h"
#include "../includes/player.h"

using namespace std;

DownloadAbility::DownloadAbility(Player* owner) : Ability(owner) {}

bool DownloadAbility::use(Game& game, istream& in) {
    char link;
    if (!(in >> link)) {
        cerr << "Error: download arg missing\n";
        return false;
    }

    // download the link
    int index = link - game.getNextPlayer().getBaseSymbol();
    shared_ptr<Link> downloadedLink;
    if (game.getNextPlayer().owns(link)) {
        downloadedLink = game.getNextPlayer().getOwnedLinks().at(index);
        owner->download(downloadedLink);
        game.getBoard().removeLink(downloadedLink);
    } else {
        cerr << "current player cannot download own link" << endl;
        return false;
    }

    // inc uses of this ability
    uses++;
    return true;
}

string DownloadAbility::getName() const { return "Download"; }
