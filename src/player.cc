#include "../includes/player.h"
#include <sstream>

// Each player should get initialized with:
//  5 abilities (not done)
//  8 links (4 data 4 virus, 1 of each strength)
//  2 associated server ports?
Player::Player(int id, char base) : id{id}, baseSymbol{base} {}

void Player::registerObserver(shared_ptr<View> observer) {
    observers.push_back(observer);
}

// void Player::addLink(char symbol, shared_ptr<Link> link) {
//     links.insert({symbol, link});
// }

string Player::getName() { return "Player " + to_string(id); }

void Player::print(ostream& out) {
    out << getName() << ":" << endl;
    int viruses = 0, datas = 0;
    for (auto link : downloaded) {
        viruses += link->type() == LinkType::VIRUS;
        datas += link->type() == LinkType::DATA;
    }
    out << "Downloaded: " << to_string(datas) << "D, " << to_string(viruses)
        << "V";
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
map<int, shared_ptr<Link>> Player::ownedLinks() { return links; }

void Player::init(string createlink, string createability) {
    istringstream streamlink(createlink);
    istringstream streamability(createability);

    string s;
    for (int i = 0; i < 8; i++) {
        shared_ptr<Link> newlink = make_shared<Link>();
        if (streamlink >> s && s.size() == 2) {
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

    for (char c : createability) {
        abilities[c] = false;
    }
}

char Player::base() { return baseSymbol; }

bool Player::owns(shared_ptr<Link> link) {
    for (auto i : links) {
        if (i.second == link)
            return true;
    }
    return false;
}
bool Player::owns(char link) {
    for (auto i : links) {
        if (i.second->symbol() == link)
            return true;
    }
    return false;
}