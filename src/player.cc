#include "player.h"
#include <sstream>

// Each player should get initialized with:
//  5 abilities (not done)
//  8 links (4 data 4 virus, 1 of each strength)
//  2 associated server ports?
Player::Player(int id, char base) : id{id}, baseSymbol{base} {}

void Player::registerObserver(std::shared_ptr<View> observer) {
    observers.push_back(observer);
}

// void Player::addLink(char symbol, std::shared_ptr<Link> link) {
//     links.insert({symbol, link});
// }

void Player::print(std::ostream& out) {
    out << "Player " << std::to_string(id) << ":" << std::endl;
    int viruses = 0, datas = 0;
    for (auto link : downloaded) {
        viruses += link->type() == LinkType::VIRUS;
        datas += link->type() == LinkType::DATA;
    }
    out << "Downloaded: " << std::to_string(datas) << "D, "
        << std::to_string(viruses) << "V";
    bool isfirst = true;
    out << std::endl;
    for (auto linkpair : links) {
        if (!isfirst) {
            out << " ";
        }
        isfirst = false;
        out << (char)(linkpair.first + baseSymbol) << ": " << *linkpair.second;
    }
    out << std::endl;
}
void Player::printcensored(std::ostream& out) {
    out << "Player " << std::to_string(id) << ":" << std::endl;
    out << "Downloaded: ";
    bool isfirst = true;
    for (std::shared_ptr<Link> link : downloaded) {
        if (!isfirst) {
            out << ", ";
        }
        isfirst = false;
        out << *link;
    }
    out << std::endl;
    for (auto linkpair : links) {
        if (!isfirst) {
            out << " ";
        }
        isfirst = false;
        out << (char)(linkpair.first + baseSymbol) << ": ?";
    }
    out << std::endl;
}

std::map<int, std::shared_ptr<Link>> Player::ownedLinks() { return links; }

void Player::init(std::string input) {
    std::istringstream iss(input);
    std::string s;
    for (int i = 0; i < 8; i++) {
        std::shared_ptr<Link> newlink = std::make_shared<Link>();
        if (iss >> s && s.size() == 2) {
            newlink->setOwner(id);
            if (s[0] == 'D') {
                newlink->makeData();
            } else if (s[0] == 'V') {
                newlink->makeVirus();
            } else {
                std::cerr << "input is wrong :(";
                break;
            }
            if (s[1] >= '1' && s[1] <= '4') {
                newlink->setStrength(s[1] - '1' + 1);
            } else {
                std::cerr << "input is wrong :(";
                break;
            }
        } else {
            std::cerr << "input is wrong :(";
            return;
        }
        newlink->setSymbol(i + baseSymbol);
        links.insert({i, newlink});
    }
}

char Player::base() { return baseSymbol; }

bool Player::owns(std::shared_ptr<Link> link) {
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