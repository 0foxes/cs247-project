#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>

#include "../includes/controller.h"
#include "../includes/game.h"
// #include "../includes/graphicdisplay.h"
// #include "../includes/textdisplay.h"

using namespace std;

// TODO (part b)
Controller::Controller(string link1, string link2, string ability1, string ability2, bool isGraphic)
    : game{nullptr} {
    game = make_shared<Game>(link1, link2, ability1, ability2, isGraphic);
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
    bool playing = false;

    // allow input cmds from cin or files
    stack<shared_ptr<istream>> inputs;
    inputs.push(shared_ptr<istream>(&cin, [](istream*) {})); // safe cin wrapper

    // print before we start the game itself :)
    game->printGame(cout);

    // loop all cmds from input stack. get args from *in instead of *cin
    while (!inputs.empty()) {
        string cmd;
        auto in = inputs.top();
        if (!(*in >> cmd)) {
            inputs.pop(); // closes file automatically if it was an ifstream
            continue;
        }

        if (cmd == "board") {
            game->printGame(cout);
        } else if (cmd == "move") {
            char link;
            string direction;
            if (*in >> link && *in >> direction) {
                game->moveLink(link, direction[0]);
                game->endTurn();
            }
            game->printGame(cout);
        } else if (cmd == "abilities") {
            game->printAbilities(cout);
        } else if (cmd == "sequence") {
            string filename;
            *in >> filename;

            auto file = make_shared<ifstream>(filename);
            if (file->is_open()) {
                inputs.push(file);
            } else {
                cerr << "Error: failed to open file " << filename << endl;
            }
        } else if (cmd == "ability") {
            // make current player use ability
            int abilityId;
            if (*in >> abilityId) {
                if (!game->useAbility(game->getCurrPlayerId(), abilityId, *in)) {
                    cerr << "Error: ability failed " << abilityId << endl;
                }
                cout << "Ability used" << endl;
            } else {
                cerr << "Error: ability ID missing\n";
            }
        } else if (cmd == "quit") {
            break;
        }
    }
}
