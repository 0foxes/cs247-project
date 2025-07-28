#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "link.h"
#include "view.h"
#include <map>
#include <memory>
#include <vector>

using namespace std;

class Player {
    // static int currID; // the current ID the latest player is on
    int id;                           // this player instance's ID
    map<char, bool> abilities;
    map<int, shared_ptr<Link>> links; // all links this player owns (can move)
    vector<shared_ptr<Link>>
        downloaded; // all links this players has downloaded
    vector<shared_ptr<View>> observers;
    char baseSymbol;

    // needs:
    // downloaded links list?

  public:
    // Each player should get initialized with:
    //  5 abilities
    //  8 links (4 data 4 virus, 1 of each strength)
    Player(int id, char base);
    bool hasLink(char link);
    void registerObserver(shared_ptr<View> observer);
    map<int, shared_ptr<Link>> ownedLinks();
    char base();
    void printabilities(ostream& out);
    void print(ostream& out);
    void printcensored(ostream& out);
    void addLink(char symbol, shared_ptr<Link> link);
    bool owns(shared_ptr<Link> link);
    bool owns(char link);
    // vector<shared_ptr<Link>> downloadedLinks();
    void init(string createlink = "D1 D2 D3 D4 V1 V2 V3 V4", string createability = "LFDSP");
};

// Initialize static member(s)
// int Player::currID = 0;

#endif