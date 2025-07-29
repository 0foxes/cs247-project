#ifndef __LINK_BOOST_ABILITY_H__
#define __LINK_BOOST_ABILITY_H__

#include "ability.h"

class LinkBoostAbility : public Ability {
  public:
    explicit LinkBoostAbility(Player* owner);

    // args[0] is the ID of the link to boost. must be owned by the player
    bool use(Game& game, std::istream& in) override;

    std::string getName() const override;
};

#endif // __LINK_BOOST_ABILITY_H__
