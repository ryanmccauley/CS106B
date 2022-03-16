#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"
#include "recursion.h"
using namespace std;

/*
 * You are free to edit the main in any way that works
 * for your testing/debugging purposes.
 * We will supply our own main() during grading
 */

int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }

    cout << "All done, exiting" << endl;
    return 0;
}


