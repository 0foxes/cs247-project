#include "../includes/player.h"
#include "../includes/ability.h"
#include "../includes/download_ability.h"
#include "../includes/expatriation_ability.h"
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
Player::Player(int id, char base) : id{id}, baseSymbol{base}, unsurmountable{false} {}

void Player::registerObserver(shared_ptr<View> observer) { observers.push_back(observer); }

void Player::addLink(char symbol, shared_ptr<Link> link) { links.emplace_back(link); }

int Player::getId() const { return id; }

string Player::getName() const { return "Player " + to_string(id); }

void Player::printcensored(ostream& out) {
    out << "Player " << to_string(id) << ":" << endl;
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
    for (auto i : abilities) {
        out << i.first;
        if (!i.second->isUsed()) {
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
            newlink->setOwner(id);
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
    int numAbilities = 0;
    for (char c : createAbility) {
        switch (c) {
        case 'L':
            abilities[++numAbilities] = make_shared<LinkBoostAbility>(this);
            break;
        case 'V':
            abilities[++numAbilities] = make_shared<StorkVisitationAbility>(this);
            break;
        case 'U':
            abilities[++numAbilities] = make_shared<UnsurmountableAbility>(this);
            break;
        case 'E':
            abilities[++numAbilities] = make_shared<ExpatriationAbility>(this);
            break;
        case 'S':
            abilities[++numAbilities] = make_shared<ScanAbility>(this);
            break;
        case 'P':
            abilities[++numAbilities] = make_shared<PolarizeAbility>(this);
            break;
        case 'D':
            abilities[++numAbilities] = make_shared<DownloadAbility>(this);
            break;
        case 'F':
            // firewll
            break;
        default:
            cerr << "unknown ability: " << c << endl;
            return;
        }
    }

    // notify observers of the new links
    for (shared_ptr<View> observer : observers) {
        observer->notify(id, links, downloaded);
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

    shared_ptr<Ability> ability = abilities[id];
    if (!ability) {
        cerr << "Error: ability nullptr " << id << endl;
        return false;
    }

    // abilities only use once
    if (ability->isUsed()) {
        cerr << "Error: ability " << id << " already used" << endl;
        return false;
    }

    return ability->use(game, in);
}

void Player::setUnsurmountable(bool val) { unsurmountable = val; }
bool Player::getUnsurmountable() { return unsurmountable; }
void Player::Download(shared_ptr<Link> link) { downloaded.push_back(link); }