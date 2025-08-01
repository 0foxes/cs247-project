#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>

#include "../includes/controller.h"
#include "../includes/game.h"

using namespace std;

Controller::Controller(string link1, string link2, string ability1, string ability2, bool isGraphic)
    : game{nullptr} {
    game = make_shared<Game>(link1, link2, ability1, ability2, isGraphic);
}

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
                    cout << "Error: ability failed " << abilityId << endl;
                } else {
                    cout << "Ability used" << endl;
                }
            } else {
                cout << "Error: ability ID missing\n";
            }
        } else if (cmd == "quit") {
            break;
        }

        // check win/loss after each move.
        // display of winner is done inside Game::isGameOver()
        if (game->isGameOver()) {
            cout << "Game over!" << endl;
            game->printGame(cout);
            break;
        }
    }
}
