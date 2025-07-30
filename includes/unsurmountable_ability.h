#ifndef __UNSURMOUNTABLE_ABILITY_H__
#define __UNSURMOUNTABLE_ABILITY_H__

#include "ability.h"

class UnsurmountableAbility : public Ability {
  public:
    explicit UnsurmountableAbility(Player* owner);

    bool use(Game& game, std::istream& in) override;

    std::string getName() const override;
};

#endif