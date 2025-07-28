#ifndef __LINK_H__
#define __LINK_H__

#include "error.h"
#include <string>

using namespace std;

// Link Type ENUM
enum LinkType { INVALID, DATA, VIRUS };

// Each link can be
// a Data or Virus
// on the board somewhere or downloaded by either player
// boosted or not
class Link {
    static int currId;
    // aaa
    LinkType linkType;
    int linkStrength; // must be 1-4
    // int downloadedBy; // either is a player id or is -1 (undownloaded)
    // int row, column;  // either is a cell on the board or is (-1, -1)
    // (unplaced)
    int uniqueId;
    char linkSymbol;
    int linkOwner;

    Error validity() const;

  public:
    Link();

    // something like a setter
    void makeVirus();
    void makeData();
    void setStrength(int strength);
    void setSymbol(char symbol);
    void setOwner(int id);
    // getters
    LinkType type() const;
    int strength() const;
    string toString() const;
    int id() const;
    char symbol() const;
    int owner() const;
};

ostream& operator<<(ostream& out, const Link& link);

#endif