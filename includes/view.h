#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#include <iostream>
using namespace std;

class View {
  public:
    /*
     * The Controller calls notify to
     * update the (r,c) location to be change
     */
    virtual void notify(int r, int c, char change) = 0;

    virtual void print(ostream& out) = 0;

    virtual ~View() = 0;
};

#endif
