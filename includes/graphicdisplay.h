#ifndef __GRAPHICDISPLAY_H__
#define __GRAPHICDISPLAY_H__

#include "view.h"
#include "window.h"
#include <vector>

class GraphicDisplay : public View {
    Xwindow xw;
    int cellWidth;                              // size of each square in pixels
    int gridSize;                               // size of board, assuming grid (n x n)
    std::vector<std::vector<int>> displayState; // cached state of each cell

  public:
    GraphicDisplay(int n);

    // redraw the cell for (r, c) that contains the item `change`
    void notify(int r, int c, char change) override;

    void print(std::ostream& out) override;
};

#endif
