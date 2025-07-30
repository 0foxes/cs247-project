#ifndef FIREWALL_ABILITY_H
#define FIREWALL_ABILITY_H

#include "ability.h"

class FirewallAbility : public Ability {
  public:
    explicit FirewallAbility(Player* owner);

    bool use(Game& game, std::istream& in) override;

    std::string getName() const override;
};

#endif
