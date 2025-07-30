#include "../includes/player.h"
#include "../includes/ability.h"
#include "../includes/download_ability.h"
#include "../includes/expatriation_ability.h"
#include "../includes/firewall_ability.h"
#include "../includes/game.h"
#include "../includes/link_boost_ability.h"
#include "../includes/polarize_ability.h"
#include "../includes/scan_ability.h"
#include "../includes/stork_visitation_ability.h"
#include "../includes/unsurmountable_ability.h"
#include <memory>
#include <sstream>

// Each player should get initialized with:
//  5 abilities (not done)
//  8 links (4 data 4 virus, 1 of each strength)
//  2 associated server ports?
Player::Player(int id, char base) : playerId{id}, baseSymbol{base}, unsurmountable{false} {}

void Player::registerObserver(shared_ptr<View> observer) { observers.push_back(observer); }

void Player::addLink(char symbol, shared_ptr<Link> link) { links.emplace_back(link); }

int Player::getId() const { return playerId; }

string Player::getName() const { return "Player " + to_string(playerId); }

void Player::printcensored(ostream& out) {
    out << "Player " << to_string(playerId) << ":" << endl;
    out << "Downloaded: ";
    bool isfirst = true;
    for (shared_ptr<Link> link : downloaded) {
        if (!isfirst) {
            out << ", ";
        }
        isfirst = false;
        out << *link;
    }
    out << endl;
    for (auto link : links) {
        if (!isfirst) {
            out << " ";
        }
        isfirst = false;
        out << link->toString() << ": ?";
    }
    out << endl;
}

void Player::printabilities(ostream& out) {
    for (int i = 1; i <= abilities.size(); i++) {
        out << i << ": " << abilities[i - 1]->getName() << " - ";
        if (!abilities[i - 1]->isUsed()) {
            out << " unused";
        } else {
            out << " used";
        }
        out << endl;
    }
}

vector<shared_ptr<Link>> Player::getOwnedLinks() const { return links; }

vector<shared_ptr<Link>> Player::getDownloaded() const { return downloaded; }

void Player::init(string createLink, string createAbility) {
    istringstream linkStream(createLink);

    string s;
    for (int i = 0; i < 8; i++) {
        shared_ptr<Link> newlink = make_shared<Link>();
        if (linkStream >> s && s.size() == 2) {
            newlink->setOwner(playerId);
            if (s[0] == 'D') {
                newlink->makeData();
            } else if (s[0] == 'V') {
                newlink->makeVirus();
            } else {
                cerr << "input is wrong :(";
                break;
            }
            if (s[1] >= '1' && s[1] <= '4') {
                newlink->setStrength(s[1] - '1' + 1);
            } else {
                cerr << "input is wrong :(";
                break;
            }
        } else {
            cerr << "input is wrong :(";
            return;
        }
        newlink->setSymbol(i + baseSymbol);
        addLink(i, newlink);
    }

    // create abilities ids 1-5 based on abilities string
    vector<int> count(26, 0);
    for (char c : createAbility) {
        count[c - 'A']++;
        switch (c) {
        case 'L':
            abilities.emplace_back(make_shared<LinkBoostAbility>(this));
            break;
        case 'V':
            abilities.emplace_back(make_shared<StorkVisitationAbility>(this));
            break;
        case 'U':
            abilities.emplace_back(make_shared<UnsurmountableAbility>(this));
            break;
        case 'E':
            abilities.emplace_back(make_shared<ExpatriationAbility>(this));
            break;
        case 'S':
            abilities.emplace_back(make_shared<ScanAbility>(this));
            break;
        case 'P':
            abilities.emplace_back(make_shared<PolarizeAbility>(this));
            break;
        case 'D':
            abilities.emplace_back(make_shared<DownloadAbility>(this));
            break;
        case 'F':
            abilities.emplace_back(make_shared<FirewallAbility>(this));
            break;
        default:
            cerr << "unknown ability: " << c << endl;
            return;
        }
    }
    int max_count = 0;
    for (int i : count) {
        if (i > max_count) {
            max_count = i;
        }
    }
    if (max_count > 2) {
        cerr << "cannot take more than 2 of an ability" << endl;
    }

    // notify observers of the new links
    for (shared_ptr<View> observer : observers) {
        observer->notify(playerId, links, downloaded, abilities);
    }
}

char Player::getBaseSymbol() { return baseSymbol; }

bool Player::owns(shared_ptr<Link> link) {
    for (auto i : links) {
        if (i == link)
            return true;
    }
    return false;
}
bool Player::owns(char link) {
    for (auto i : links) {
        if (i->getSymbol() == link)
            return true;
    }
    return false;
}

shared_ptr<Link> Player::getLink(char symbol) {
    for (const auto& link : links) {
        if (link->getSymbol() == symbol) {
            return link;
        }
    }
    return nullptr;
}

bool Player::useAbility(int id, istream& in, Game& game) {
    if (id > abilities.size()) {
        cerr << "Error: invalid ability ID " << id << endl;
        return false;
    }

    // input is 1-indexed so subtract 1
    shared_ptr<Ability> ability = abilities[id - 1];
    if (!ability) {
        cerr << "Error: ability nullptr " << id << endl;
        return false;
    }

    // abilities only use once
    if (ability->isUsed()) {
        cerr << "Error: ability " << id << " already used" << endl;
        return false;
    }

    bool result = ability->use(game, in);
    // notify observers of ability use and result
    for (shared_ptr<View> observer : observers) {
        observer->notify(game.getCurrPlayerId(), links, downloaded, abilities);
        observer->notify(game.getNextPlayerId(), game.getNextPlayer().links,
                         game.getNextPlayer().downloaded, game.getNextPlayer().abilities);
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                game.getBoard().notifyObservers(i, j);
            }
        }
    }
    return result;
}

void Player::setUnsurmountable(bool val) { unsurmountable = val; }
bool Player::getUnsurmountable() { return unsurmountable; }

void Player::download(shared_ptr<Link> link) {
    downloaded.push_back(link);
    // notify observers of the new downloaded link
    for (shared_ptr<View> observer : observers) {
        observer->notify(playerId, links, downloaded, abilities);
    }
}
