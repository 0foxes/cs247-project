#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "link.h"
#include "view.h"
#include <map>
#include <memory>
#include <vector>

class Player {
    // static int currID; // the current ID the latest player is on
    int id; // this player instance's ID
    std::map<int, std::shared_ptr<Link>>
        links; // all links this player owns (can move)
    std::vector<std::shared_ptr<Link>>
        downloaded; // all links this players has downloaded
    std::vector<std::shared_ptr<View>> observers;
    char baseSymbol;

    // needs:
    // abilities list
    // downloaded links list?

  public:
    // Each player should get initialized with:
    //  5 abilities
    //  8 links (4 data 4 virus, 1 of each strength)
    Player(int id, char base);
    bool hasLink(char link);
    void registerObserver(std::shared_ptr<View> observer);
    std::map<int, std::shared_ptr<Link>> ownedLinks();
    char base();
    void print(std::ostream& out);
    void printcensored(std::ostream& out);
    void addLink(char symbol, std::shared_ptr<Link> link);
    bool owns(std::shared_ptr<Link> link);
    bool owns(char link);
    // std::vector<std::shared_ptr<Link>> downloadedLinks();
    void init(std::string input = "D1 D2 D3 D4 V1 V2 V3 V4");
};

// Initialize static member(s)
// int Player::currID = 0;

#endif