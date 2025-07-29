#include "player.h"
#include "ability.h"
#include "link_boost_ability.h"
#include <sstream>

// Each player should get initialized with:
//  5 abilities (not done)
//  8 links (4 data 4 virus, 1 of each strength)
//  2 associated server ports?
Player::Player(int id, char base) : id{id}, baseSymbol{base} {}

void Player::registerObserver(shared_ptr<View> observer) { observers.push_back(observer); }

// void Player::addLink(char symbol, shared_ptr<Link> link) {
//     links.insert({symbol, link});
// }

string Player::getName() { return "Player " + to_string(id); }

void Player::print(ostream& out) {
    out << getName() << ":" << endl;
    int viruses = 0, datas = 0;
    for (auto link : downloaded) {
        viruses += link->getType() == LinkType::VIRUS;
        datas += link->getType() == LinkType::DATA;
    }
    out << "Downloaded: " << to_string(datas) << "D, " << to_string(viruses) << "V";
    bool isfirst = true;
    out << endl;
    for (auto linkpair : links) {
        if (!isfirst) {
            out << " ";
        }
        isfirst = false;
        out << (char)(linkpair.first + baseSymbol) << ": " << *linkpair.second;
    }
    out << endl;
}

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
    for (auto linkpair : links) {
        if (!isfirst) {
            out << " ";
        }
        isfirst = false;
        out << (char)(linkpair.first + baseSymbol) << ": ?";
    }
    out << endl;
}

void Player::printabilities(ostream& out) {
    for (auto i : abilities) {
        out << i.first;
        if (!i.second) {
            out << " unused";
        } else {
            out << " used";
        }
        out << endl;
    }
}
map<int, shared_ptr<Link>> Player::getOwnedLinks() { return links; }

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
        links.insert({i, newlink});
    }

    // create abilities ids 1-5 based on abilities string
    int numAbilities = 0;
    for (char c : createAbility) {
        switch (c) {
        case 'L':
            abilities[++numAbilities] = make_shared<LinkBoostAbility>(this);
            break;
        case 'V':
            // stork
            break;
        case 'U':
            // unsurmountable
            break;
        case 'E':
            // expat
            break;
        case 'S':
            // scan
            break;
        case 'P':
            // polarize
            break;
        case 'D':
            // download
            break;
        case 'F':
            // firewll
            break;
        default:
            cerr << "unknown ability: " << c << endl;
            return;
        }
    }
}

char Player::getBaseSymbol() { return baseSymbol; }

bool Player::owns(shared_ptr<Link> link) {
    for (auto i : links) {
        if (i.second == link)
            return true;
    }
    return false;
}
bool Player::owns(char link) {
    for (auto i : links) {
        if (i.second->getSymbol() == link)
            return true;
    }
    return false;
}

shared_ptr<Link> Player::getLink(char symbol) {
    for (const auto& [id, link] : links) {
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
