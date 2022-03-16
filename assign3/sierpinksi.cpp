/*
 * This file contains all of the methods that
 * protain to drawing sierpinksi triangles.
 */
#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * This function draws a filled, black triangle on the provided GWindow
 * that is defined by the corner GPoints one, two, and three. This
 * function has been provided for you and does not need to be modified.
 */
void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}

GPoint getMidpoint(GPoint a, GPoint b) {
    return {
        (a.x + b.x) / 2, (a.y + b.y) / 2
    };
}

/* This function draws the sierpinksi triangles
 * recursively on the screen.
 */
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {
    if (order == 0) {
        fillBlackTriangle(window, one, two, three);
        return 1;
    } else {
        return drawSierpinskiTriangle(window, one, { getMidpoint(one, two).x, two.y }, { getMidpoint(one, three).x, getMidpoint(one, three).y }, order - 1)
                + drawSierpinskiTriangle(window, { getMidpoint(one, two).x, one.y }, two, { getMidpoint(two, three).x, getMidpoint(two, three).y }, order - 1)
                + drawSierpinskiTriangle(window, { getMidpoint(one, three).x, getMidpoint(one, three).y }, { getMidpoint(two, three).x, getMidpoint(two, three).y }, three, order - 1);
    }
}


/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

