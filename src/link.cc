#include "../includes/link.h"

// static tings
int Link::currId = 0;

Link::Link()
    : linkType{LinkType::INVALID}, linkStrength{0}, uniqueId{currId++},
      linkSymbol{0}, linkOwner{-1}, isBoosted{false} {
    // creates an "invalid" link
    // need to do more steps to set up after this
}

// setters?
void Link::makeVirus() { linkType = LinkType::VIRUS; }
void Link::makeData() { linkType = LinkType::DATA; }
void Link::setStrength(int strength) { linkStrength = strength; }
void Link::setSymbol(char symbol) { linkSymbol = symbol; }

// getters
LinkType Link::getType() const { return linkType; }
int Link::getStrength() const { return linkStrength; }

Error Link::validity() const {
    string errorString = "";
    bool isValid = true;
    if (linkType == LinkType::INVALID) {
        errorString += "Link type is INVALID, ";
        isValid = false;
    } //&& strength >= 1 && strength <= 4;
    if (linkStrength < 1) {
        errorString += "Link strength must be >= 1, ";
        isValid = false;
    }
    if (linkStrength > 4) {
        errorString += "Link strength must be <= 4, ";
        isValid = false;
    }

    // return error if there is one
    if (isValid) {
        return Error();
    }
    return Error(ErrorType::INVALID_OBJECT_INSTANCE, errorString);
}

string Link::toString() const {
    string linkString = "";
    // linkString += to_string(uniqueId);
    if (linkType == LinkType::DATA) {
        linkString += "D";
    } else if (linkType == LinkType::VIRUS) {
        linkString += "V";
    } else if (linkType == LinkType::INVALID) {
        linkString += "INVALID";
    }
    return linkString + to_string(this->getStrength());
}

char Link::getSymbol() const { return linkSymbol; }

int Link::getOwner() const { return linkOwner; }

void Link::setOwner(int id) { linkOwner = id; }

ostream& operator<<(ostream& out, const Link& link) {
    return out << link.toString();
}
