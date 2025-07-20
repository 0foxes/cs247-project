#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "game.h"
#include "view.h"
#include <iostream>
#include <memory>

// More documentation in controller.cc file

class Controller {

    // bool isGraphic;

    // View* display; // The text (or graphical!) display.

    std::shared_ptr<Game> game;

    // void init(std::istream& input, Game& g);

    /// Helper to check if the game is in a win state
    // if yes, prints Won to stdout and returns true
    // if no, returns false
    // bool checkWin(int movesLeft);

  public:
    Controller(std::string link1, std::string link2, bool isGraphic);
    // Controller(bool graphics = false);
    // ~Controller();
    void play();
    // void notify(int r, int c, int state);
};

#endif
