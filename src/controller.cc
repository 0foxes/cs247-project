#include "controller.h"
#include "game.h"
// #include "graphicdisplay.h"
#include "textdisplay.h"
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// TODO (part b)
Controller::Controller(std::string link1, std::string link2, bool isGraphic)
    : game{nullptr} {
    game = std::make_shared<Game>(link1, link2, isGraphic);
    // else(){

    // }
}
// Controller::Controller(bool b) : isGraphic{b}, td{nullptr}, game{new Game()}
// {}

// Controller::~Controller() {
//     delete td;
//     delete game;
// }

/*
 * Update the View(s) to indicate the new game state
 */
// void Controller::notify(int r, int c, int state) {
//     if (td)
//         td->notify(r, c, state);
// }

/*
 * Called when command init is received
 */
// void Controller::init(istream& input, Game& g) {
//     int r, c, s;
//     while (input >> r >> c && r != -1 && c != -1) {
//         input >> s;
//         g.init(r, c, s);
//     }
// }

// bool Controller::checkWin(int movesLeft) {
//     if (game->isWon() && movesLeft != 0) {
//         cout << "Won" << endl;
//         return true;
//     }
//     return false;
// }

void Controller::play() {
    int moves = 0;
    string cmd;
    bool playing = false;
    // print before we start the game itself :)
    game->printGame(std::cout);

    while (cin >> cmd) {
        if (cmd == "board") {
            game->printGame(std::cout);
        } else if (cmd == "move") {
            char link;
            std::string direction;
            if (cin >> link && cin >> direction) {
                game->moveLink(link, direction[0]);
            }
            game->printGame(std::cout);
        } else if (cmd == "abilities") {
            cout << "unimplemented\n";
        } else if (cmd == "abilities") {
            int id;
            cin >> id;
            cout << "unimplemented\n";
        } else if (cmd == "sequence") {
            string file;
            cin >> file;
            cout << "unimplemented\n";
        } else if (cmd == "quit") {
            break;
        }
    }
}
