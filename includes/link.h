#ifndef __LINK_H__
#define __LINK_H__

#include "error.h"
#include <string>

using namespace std;

// Link LinkType ENUM
enum LinkType { INVALID, DATA, VIRUS };

// Each link can be
// - a Data or Virus
// - on the board somewhere or downloaded by either player
// - boosted or not
class Link {
    static int currId;
    // aaa
    LinkType linkType;
    int linkStrength; // must be 1-4
    int downloadedBy; // either is a player id or is -1 (undownloaded)
    int uniqueId;
    char linkSymbol;
    int linkOwner;

    // true if it has the boosted ability (move 1 extra cell)
    bool isBoosted;

    Error validity() const;

  public:
    Link();

    // something like a setter
    void makeVirus();
    void makeData();
    void setStrength(int strength);
    void setType(LinkType type);
    void setSymbol(char symbol);
    void setOwner(int id);
    void setBoosted(bool boosted) { isBoosted = boosted; }

    // getters
    LinkType getType() const;
    int getStrength() const;
    string toString() const;
    int getId() const;
    char getSymbol() const;
    int getOwner() const;
    bool getIsBoosted() const { return isBoosted; }
};

ostream& operator<<(ostream& out, const Link& link);

#endif