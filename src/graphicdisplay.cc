#include "graphicdisplay.h"

GraphicDisplay::GraphicDisplay(int n)
    : gridSize{n}, cellWidth{500 / n}, displayState(n, std::vector<int>(n, 0)) {
    // white grid with black outlines
    for (int r = 0; r < gridSize; r++) {
        for (int c = 0; c < gridSize; c++) {
            xw.outlineRectangle(c * cellWidth, r * cellWidth, cellWidth, cellWidth, Xwindow::Black);
        }
    }
}

void GraphicDisplay::notify(int r, int c, char change) {
    if (r < 0 || r >= gridSize || c < 0 || c >= gridSize) {
        std::cerr << "invalid notif";
        return;
    }
    displayState[r][c] = change;

    int color;
    switch (change) {
    case 0:
        color = Xwindow::White;
        break;
    case 1:
        color = Xwindow::Black;
        break;
    case 2:
        color = Xwindow::Red;
        break;
    case 3:
        color = Xwindow::Green;
        break;
    case 4:
        color = Xwindow::Blue;
        break;
    }

    xw.fillRectangle(c * cellWidth, r * cellWidth, cellWidth, cellWidth, color);
}

void GraphicDisplay::print(std::ostream& out) {}
