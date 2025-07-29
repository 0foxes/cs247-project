#ifndef __STORK_VISITATION_ABILITY_H__
#define __STORK_VISITATION_ABILITY_H__

#include "ability.h"

class StorkVisitationAbility : public Ability {
  public:
    explicit StorkVisitationAbility(Player* owner);

    // args[0] is the ID of the link to boost. must be owned by the player
    bool use(Game& game, std::istream& in) override;

    std::string getName() const override;
};

#endif
