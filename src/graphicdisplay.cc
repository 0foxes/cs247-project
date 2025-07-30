#include "../includes/graphicdisplay.h"

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
    int yBase = (playerId == 0) ? 500 : 500 + 100;

    const auto& ps = playerStates[playerId];

    // clear player info background
    xw.fillRectangle(0, yBase, 500, 100, Xwindow::White);

    // header
    // player number is 1-indexed in display
    xw.drawString(10, yBase + lineHeight, "Player " + to_string(playerId + 1) + ":");

    // abilities
    int abilitiesAvail = 0;
    for (auto& ability : ps.abilities) {
        if (!ability->isUsed()) {
            abilitiesAvail++;
        }
    }
    xw.drawString(10, yBase + 2 * lineHeight, "Abilities: " + to_string(abilitiesAvail));

    // downloads
    int dataCount = 0, virusCount = 0;
    for (auto& download : ps.downloaded) {
        if (download->getType() == LinkType::DATA) {
            dataCount++;
        } else if (download->getType() == LinkType::VIRUS) {
            virusCount++;
        }
    }
    xw.drawString(10, yBase + 3 * lineHeight,
                  "Downloaded: " + to_string(dataCount) + "D, " + to_string(virusCount) + "V");

    // links
    string linkStr;
    bool isfirst = true;
    for (auto link : ps.links) {
        if (!isfirst) {
            linkStr = linkStr + " ";
        }
        isfirst = false;
        // only print opponent links if they are revealed or its the current player's
        if (link->getIsRevealed() || playerId == currPlayerId) {
            linkStr = linkStr + link->getSymbol() + ": " + link->toString();
        } else {
            linkStr = linkStr + link->getSymbol() + ": ?";
        }
    }

    xw.drawString(10, yBase + 4 * lineHeight, linkStr);
}

void GraphicDisplay::notify(int r, int c, CellState cell) {
    char itemSymbol = cell.link;

    if (r < 0 || r >= gridSize || c < 0 || c >= gridSize) {
        cerr << "invalid notif";
        return;
    }

    // prioritize displaying link, then firewall and serverport
    if (itemSymbol != '.') {
        itemSymbol = cell.link;
    } else if (cell.hasFirewall) {
        // firewall symbol upsidedown for player 1
        itemSymbol = (cell.firewallOwnerId == 1) ? 'm' : 'w';
    } else if (cell.isServerPort) {
        itemSymbol = 'S';
    }
    displayState[r][c] = itemSymbol;

    // erase old item
    xw.fillRectangle(c * cellWidth + 5, r * cellWidth + 5, cellWidth - 10, cellWidth - 10,
                     Xwindow::White);
    // draw new item symbol or blank if nothing there
    if (cell.linkType == 1 && cell.isRevealed) {
        xw.fillRectangle(c * cellWidth + 5, r * cellWidth + 5, cellWidth - 10, cellWidth - 10,
                         Xwindow::Green);
    } else if (cell.linkType == 2 && cell.isRevealed) {
        xw.fillRectangle(c * cellWidth + 5, r * cellWidth + 5, cellWidth - 10, cellWidth - 10,
                         Xwindow::Red);
    }

    xw.drawString((c * cellWidth) + (cellWidth / 2), (r * cellWidth) + (cellWidth / 2),
                  (itemSymbol == '.') ? "" : string(1, itemSymbol));
}

void GraphicDisplay::notify(int playerId, vector<shared_ptr<Link>> links,
                            vector<shared_ptr<Link>> downloads,
                            vector<shared_ptr<Ability>> abilities) {
    PlayerState& ps = playerStates[playerId];

    // update player state
    ps.links = links;
    ps.downloaded = downloads;
    ps.abilities = abilities;

    // redraw
    drawPlayerState(playerId);
}

void GraphicDisplay::print(ostream& out) {
    // do nothing
}
