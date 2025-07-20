#include "textdisplay.h"

TextDisplay::TextDisplay()
    : theBoard{std::vector<std::vector<char>>(8, std::vector<char>(8, '.'))} {}

/*
 * updates the symbol at position r, c
 */
// for now there is no way to update player state since its not a part of the
// display rn
void TextDisplay::notify(int r, int c, char change) {
    theBoard.at(r).at(c) = change;
}

/*
 * Prints the grid as specified in the assignment specification.
 */
void TextDisplay::print(std::ostream& out) {
    for (std::vector<char> row : theBoard) {
        for (char element : row) {
            out << element;
        }
        out << "\n";
    }
}
