#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "ability.h"
#include "link.h"
#include "view.h"

using namespace std;

class Player {
    // static int currID; // the current ID the latest player is on
    int id; // this player instance's ID
    // map of abilities this player owns. key is the ability's ID (1-5)
    vector<shared_ptr<Ability>> abilities;
    vector<shared_ptr<Link>> links; // all links this player owns (can move)
    // all links this players has downloaded
    vector<shared_ptr<Link>> downloaded;
    vector<shared_ptr<View>> observers;
    char baseSymbol;
    bool unsurmountable;

    // needs:
    // downloaded links list?

  public:
    // Each player should get initialized with:
    //  5 abilities
    //  8 links (4 data 4 virus, 1 of each strength)
    Player(int id, char base);
    bool hasLink(char link);
    void registerObserver(shared_ptr<View> observer);
    char getBaseSymbol();

    void printabilities(ostream& out);
    void printcensored(ostream& out);
    string getName() const;
    int getId() const;

    void addLink(char symbol, shared_ptr<Link> link);

    vector<shared_ptr<Link>> getOwnedLinks() const;
    bool owns(shared_ptr<Link> link);
    bool owns(char link);
    // get specific link owned by this player. returns null if link isn't owned
    shared_ptr<Link> getLink(char symbol);

    vector<shared_ptr<Link>> getDownloaded() const;

    void init(string createlink = "D1 D2 D3 D4 V1 V2 V3 V4", string createability = "LFDSP");

    // return false if issue using ability.
    // id is the ability ID (1-5). in is ability args
    bool useAbility(int id, istream& in, Game& game);

    // Initialize static member(s)
    // int Player::currID = 0;
    void setUnsurmountable(bool val);
    bool getUnsurmountable();
    void download(shared_ptr<Link> link);
};

#endif