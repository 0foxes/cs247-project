#ifndef __EXPATRIATION_ABILITY_H__
#define __EXPATRIATION_ABILITY_H__

#include "ability.h"

class ExpatriationAbility : public Ability {
  public:
    explicit ExpatriationAbility(Player* owner);

    bool use(Game& game, std::istream& in) override;

    std::string getName() const override;
};

#endif
