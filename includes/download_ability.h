#ifndef __DOWNLOAD_ABILITY_H__
#define __DOWNLOAD_ABILITY_H__

#include "ability.h"

class DownloadAbility : public Ability {
  public:
    explicit DownloadAbility(Player* owner);

    bool use(Game& game, std::istream& in) override;

    std::string getName() const override;
};

#endif