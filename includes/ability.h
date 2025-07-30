#ifndef __ABILITY_H__
#define __ABILITY_H__

#include <string>
#include <vector>

class Game;
class Player;

class Ability {
  protected:
    Player* owner; // the player who owns this ability
    int uses = 0;

  public:
    explicit Ability(Player* owner) : owner(owner) {}
    virtual ~Ability() = default;

    // use the ability in the given game. istream is used to read args.
    // returns true if the ability was successfully used, false if error
    virtual bool use(Game& game, std::istream& in) = 0;
    virtual std::string getName() const = 0;

    // returns true if this ability has been used at least once
    bool isUsed() const { return (uses > 0); }
    Player* getOwner() const { return owner; }
};

#endif
