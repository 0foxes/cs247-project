#include "../includes/controller.h"
// #include "game.h"
// #include "textdisplay.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

/*
 * TODO: update main function to take argument "-graphics" for part b.
 * If -graphics argument is provided, forward this to the controller
 */

// ./raiinet -ability1 <file> -ability2 <file> -link1 <file> -link2 <file>
// -graphics

// these all have defaults

int main(int argc, char* argv[]) {
    bool useGraphics = false;
    string link1 = "D1 D2 D3 D4 V1 V2 V3 V4";
    string link2 = "D1 D2 D3 D4 V1 V2 V3 V4";
    string ability1 = "LFDSP";
    string ability2 = "LFDSP";
    string link1file, link2file, ability1file, ability2file;

    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "-graphics") {
            useGraphics = true;
        } else if (string(argv[i]) == "-link1") {
            link1file = string(argv[++i]);
            ifstream input(link1file.c_str());
            stringstream buffer;
            buffer << input.rdbuf();
            link1 = buffer.str();
        } else if (string(argv[i]) == "-link2") {
            link2file = string(argv[++i]);
            ifstream input(link2file.c_str());
            stringstream buffer;
            buffer << input.rdbuf();
            link2 = buffer.str();
        } else if (string(argv[i]) == "-ability1") {
            ability1 = argv[++i];
        } else if (string(argv[i]) == "-ability2") {
            ability2 = argv[++i];
        }
    }

    // Controller c(useGraphics);
    // c.play();
    // Controller c;
    // c.play();

    // cout << Error();
    // cout << Error(ErrorType::INVALID_PARAMETERS);
    // cout << Error(ErrorType::INVALID_PARAMETERS,
    //                    "Link cannot have strength greater than 4");
    // TextDisplay textdisplay;
    Controller controller(link1, link2, ability1, ability2, useGraphics);
    // TextDisplay textdisplay;
    // game.registerObserver(make_shared<TextDisplay>());
    // game.printGame(cout);
    // game.moveLink('a', 'd');
    // game.printGame(cout);
    controller.play();
}
