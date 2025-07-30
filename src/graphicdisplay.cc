#include "graphicdisplay.h"

// note: init xw with space for 500x500 grid + player info sections
GraphicDisplay::GraphicDisplay(int n)
    : gridSize{n}, cellWidth{500 / n}, displayState(n, vector<int>(n, 0)),
      xw{Xwindow(500, 500 + 100 + 100)} {
    // white grid with black outlines
    for (int r = 0; r < gridSize; r++) {
        for (int c = 0; c < gridSize; c++) {
            xw.outlineRectangle(c * cellWidth, r * cellWidth, cellWidth, cellWidth, Xwindow::Black);
        }
    }
}

void GraphicDisplay::drawPlayerState(int playerId) {
    const int lineHeight = 16;
    int yBase = (playerId == 1) ? 500 : 500 + 100;

    const auto& ps = playerStates[playerId];

    // clear player info background
    xw.fillRectangle(0, yBase, 500, 100, Xwindow::White);

    // header
    xw.drawString(10, yBase + lineHeight, "Player " + to_string(playerId) + ":");

    // downloads
    int dataCount = 0, virusCount = 0;
    for (auto& download : ps.downloaded) {
        if (download->getType() == LinkType::DATA) {
            dataCount++;
        } else if (download->getType() == LinkType::VIRUS) {
            virusCount++;
        }
    }
    xw.drawString(10, yBase + 2 * lineHeight,
                  "Downloaded: " + to_string(dataCount) + "D, " + to_string(virusCount) + "V");

    // links
    string linkStr;
    bool isfirst = true;
    for (auto link : ps.links) {
        if (!isfirst) {
            linkStr = linkStr + " ";
        }
        isfirst = false;
        linkStr = linkStr + link->getSymbol() + ": " + link->toString();
    }

    xw.drawString(10, yBase + 3 * lineHeight, linkStr);
}

void GraphicDisplay::notify(int r, int c, char change) {
    string itemStr(1, change);
    if (change == '.') {
        itemStr = ""; // empty cell
    }

    if (r < 0 || r >= gridSize || c < 0 || c >= gridSize) {
        cerr << "invalid notif";
        return;
    }
    displayState[r][c] = change;

    // erase old item
    xw.fillRectangle(c * cellWidth + 5, r * cellWidth + 5, cellWidth - 10, cellWidth - 10,
                     Xwindow::White);
    xw.drawString((c * cellWidth) + (cellWidth / 2), (r * cellWidth) + (cellWidth / 2), itemStr);
}

void GraphicDisplay::notify(int playerId, vector<shared_ptr<Link>> links,
                            vector<shared_ptr<Link>> downloads) {
    PlayerState& ps = playerStates[playerId];

    // update player state
    ps.links = links;
    ps.downloaded = downloads;

    // redraw
    drawPlayerState(playerId);
}

void GraphicDisplay::print(ostream& out) {
    // do nothing
}
