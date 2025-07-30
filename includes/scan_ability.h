#ifndef __SCAN_ABILITY_H__
#define __SCAN_ABILITY_H__

#include "ability.h"

class ScanAbility : public Ability {
  public:
    explicit ScanAbility(Player* owner);

    bool use(Game& game, std::istream& in) override;

    std::string getName() const override;
};

#endif
