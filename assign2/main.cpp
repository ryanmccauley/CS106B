#include <iostream>
#include "console.h"
#include "testing/SimpleTest.h"
#include "maze.h"
#include "search.h"
#include "queue.h"
#include "stack.h"
#include "grid.h"
using namespace std;

// You are free to edit the main in any way that works
// for your testing/debugging purposes.
// We will supply our main() during grading

Vector<string> fruits = { "apple", "orange", "melon", "banana", "strawberry" };

bool valid(Grid<bool> &g, GridLocation loc) {
    return g.inBounds(loc);
}

GridLocation getMoveFromDirection(char c, GridLocation loc) {
    switch (c) {
        case 'n':
            return { loc.row - 1, loc.col };
        case 's':
            return { loc.row + 1, loc.col };
        case 'e':
            return { loc.row, loc.col + 1 };
        case 'w':
            return { loc.row, loc.col - 1 };
        default:
            return loc;
    }
}

Set<char> getMoves(Grid<bool> &g, GridLocation loc) {
    Set<char> directions = { 'n', 's', 'e', 'w' };
    Set<char> possibleMoves;

    for (char direction : directions) {
        GridLocation move = getMoveFromDirection(direction, loc);
        if (valid(g, move)) {
            possibleMoves.add(direction);
        }
    }

    return possibleMoves;
}

void allPathsHelper(Grid<bool> &g, Vector<string> &paths, Set<GridLocation> traveled, GridLocation loc, GridLocation finish, string currPath) {
    if (loc == finish) {
        paths.add(currPath);
    } else {
        traveled.add(loc);

        for (char c : getMoves(g, loc)) {
            GridLocation move = getMoveFromDirection(c, loc);
            if (!traveled.contains(move)) {
                allPathsHelper(g, paths, traveled, move, finish, currPath + c);
            }
        }
    }
}

Vector<string> allPaths(Grid<bool> &g) {
    Vector<string> paths;

    allPathsHelper(g, paths, {}, { 0, 0 }, { g.numRows() - 1, g.numCols() - 1 }, "");

    return paths;
}

int main()
{
    if (runSimpleTests(NO_TESTS)) {
        return 0;
    }

    /*Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    solveMaze(maze);

    searchEngine("res/tiny.txt");*/

    Grid<bool> g(2, 2, false);

    cout << g;

    for (int i = 0; i < 4; i++) {
        cout << endl;
    }

    cout << allPaths(g);

    cout << endl << "All done!" << endl;
    return 0;
}


// Do not remove or edit this test. It is here to to confirm that your code
// conforms to the expected function prototypes needed for grading
PROVIDED_TEST("Confirm function prototypes") {
    bool execute = false;
    if (execute) {
        Grid<bool> g;
        GridLocation loc;
        Set<GridLocation> set;
        Stack<GridLocation> path;
        string str;

        set = generateValidMoves(g, loc);
        checkSolution(g, path);
        readMazeFile(str, g);
        readSolutionFile(str, path);
        path = solveMaze(g);

        Set<string> strset;
        Map<std::string, Set<std::string>> map;
        str = cleanToken(str);
        strset = gatherTokens(str);
        buildIndex(str, map);
        strset = findQueryMatches(map, str);
        searchEngine(str);
    }
}
