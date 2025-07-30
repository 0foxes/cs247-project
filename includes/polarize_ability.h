#ifndef __POLARIZE_ABILITY_H__
#define __POLARIZE_ABILITY_H__

#include "ability.h"

class PolarizeAbility : public Ability {
  public:
    explicit PolarizeAbility(Player* owner);

    bool use(Game& game, std::istream& in) override;

    std::string getName() const override;
};

#endif
