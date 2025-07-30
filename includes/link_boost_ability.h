#ifndef __LINK_BOOST_ABILITY_H__
#define __LINK_BOOST_ABILITY_H__

#include "ability.h"

class LinkBoostAbility : public Ability {
  public:
    explicit LinkBoostAbility(Player* owner);

    bool use(Game& game, std::istream& in) override;

    std::string getName() const override;
};

#endif
