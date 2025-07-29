#ifndef __SCAN_ABILITY_H__
#define __SCAN_ABILITY_H__

#include "ability.h"

class ScanAbility : public Ability {
  public:
    explicit ScanAbility(Player* owner);

    // args[0] is the ID of the link to boost. must be owned by the player
    bool use(Game& game, std::istream& in) override;

    std::string getName() const override;
};

#endif
