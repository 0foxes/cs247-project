#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "game.h"
#include "view.h"
#include <iostream>
#include <memory>

using namespace std;

class Controller {

    shared_ptr<Game> game;

  public:
    Controller(string link1, string link2, string ability1, string ability2, bool isGraphic);
    void play();
};

#endif
