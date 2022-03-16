/* This file is used to solve maze files in the res/ folder.
 * Included is generateValidMoves, checkSolution, & solveMaze methods
 */
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* This function returns the possible valid moves based on a origin
 * GridLocation value `cur`.
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;

    // Add N, S, E, W locations
    neighbors.add({ cur.row + 1, cur.col });
    neighbors.add({ cur.row - 1, cur.col });
    neighbors.add({ cur.row, cur.col + 1 });
    neighbors.add({ cur.row, cur.col - 1});

    // Store valid locations to return
    Set<GridLocation> validLocations;
    for (GridLocation loc : neighbors) {
        if (maze.inBounds(loc.row, loc.col) && maze[loc.row][loc.col]) {
            validLocations.add(loc);
        }
    }

    neighbors = neighbors.intersect(validLocations);

    return neighbors;
}

/* This function checks a Stack of GridLocations named `path`
 * to see if it satisifes the maze.
 */
void checkSolution(Grid<bool>& maze, Stack<GridLocation> path) {
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};

    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }

    Set<GridLocation> traveled;

    // Store endLocation as the first element off of the stack and lastLocation gets initialized to the starting position once the path is empty.
    GridLocation lastLocation = path.peek();
    GridLocation endLocation = path.peek();

    while (!path.isEmpty()) {
        GridLocation next = path.pop();
        if ((lastLocation != next && !generateValidMoves(maze, lastLocation).contains(next)) || traveled.contains(next)) {
            error("Invalid move to new location");
        }
        traveled.add(next);
        lastLocation = next;
    }

    GridLocation start = { 0, 0 };
    GridLocation finish = { maze.numRows() - 1, maze.numCols() - 1 };

    // Check for not beginning at start
    if (lastLocation != start) {
        error("Invalid start location");
    }

    // Check for not beginning at start
    if (endLocation != finish) {
        error("Invalid end location");
    }
}

/* This function implements the breadth first search
 * algorithm to quickly solve a maze.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);

    Set<GridLocation> visited;
    Queue<Stack<GridLocation>> paths;
    GridLocation start;
    GridLocation exit = { maze.numRows() - 1, maze.numCols() - 1 };
    paths.enqueue({ start });

    Stack<GridLocation> found;

    while (!paths.isEmpty()) {
        Stack<GridLocation> path = paths.dequeue();
        if (path.peek() == exit) {
            found = path;
            MazeGraphics::highlightPath(path, "green");
            break;
        } else {
            MazeGraphics::highlightPath(path, "blue");
            Set<GridLocation> neighbors = generateValidMoves(maze, path.peek());
            for (GridLocation loc : neighbors) {
                Stack<GridLocation> copy = path;
                copy.push(loc);
                if (!visited.contains(loc)) {
                    paths.enqueue(copy);
                    visited.add(loc);
                }
            }
        }
    }

    return found;
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* This provided functions opens and read the contents of files ending
 * in a .soln extension and interprets the contents as a Stack of
 * GridLocations, populating the provided soln data structure.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> neighbors = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, center));
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> neighbors = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, side));
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> neighbors = {{1, 0}};

    EXPECT_EQUAL(neighbors, generateValidMoves(maze, corner));
}

PROVIDED_TEST("checkSolution on correct path") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on correct path loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("checkSolution on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };

    EXPECT_ERROR(checkSolution(maze, not_end_at_exit));
    EXPECT_ERROR(checkSolution(maze, not_begin_at_entry));
    EXPECT_ERROR(checkSolution(maze, go_through_wall));
    EXPECT_ERROR(checkSolution(maze, teleport));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x35") {
    Grid<bool> maze;
    readMazeFile("res/21x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(checkSolution(maze, soln));
}

STUDENT_TEST("generateValidMoves test case") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    GridLocation outside_of_board = { -500, -500 };
    GridLocation corner = { 0, 0 };
    GridLocation wall = { 1, 0 };
    Set<GridLocation> empty;
    EXPECT_EQUAL(generateValidMoves(maze, outside_of_board), empty);
    Set<GridLocation> valid = { { 0, 1 }, { 1, 0 } };
    EXPECT_EQUAL(generateValidMoves(maze, corner), valid);
    valid = { { 0, 0 }, { 2, 0 } };
    EXPECT_EQUAL(generateValidMoves(maze, wall), valid);
}

STUDENT_TEST("checkSolution test cases") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);

    Stack<GridLocation> path = { { 0, 0 } , { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 }, { 0, 6 }, { 1, 6 }, { 2,6 }, { 3, 6 }, { 4, 6 }};
    EXPECT_NO_ERROR(checkSolution(maze, path));

    // Invalid starting point
    path = { { 1, 1 } , { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 }, { 0, 6 }, { 1, 6 }, { 2,6 }, { 3, 6 }, { 4, 6 }};
    EXPECT_ERROR(checkSolution(maze, path));

    // Invalid ending point
    EXPECT_ERROR(checkSolution(maze, path));

    // Duplicates
    path = { { 1, 1 } , { 1, 1 }, { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 }, { 0, 6 }, { 1, 6 }, { 2,6 }, { 3, 6 }, { 4, 6 }};
    EXPECT_ERROR(checkSolution(maze, path));
}

STUDENT_TEST("solveMaze test cases") {
    Grid<bool> maze;
    readMazeFile("res/33x41.maze", maze);
    Stack<GridLocation> sol = solveMaze(maze);
    EXPECT_NO_ERROR(checkSolution(maze, sol));

    readMazeFile("res/19x35.maze", maze);
    sol = solveMaze(maze);
    EXPECT_NO_ERROR(checkSolution(maze, sol));

    readMazeFile("res/17x41.maze", maze);
    sol = solveMaze(maze);
    EXPECT_NO_ERROR(checkSolution(maze, sol));

    readMazeFile("res/13x39.maze", maze);
    sol = solveMaze(maze);
    EXPECT_NO_ERROR(checkSolution(maze, sol));

    readMazeFile("res/2x2.maze", maze);
    sol = solveMaze(maze);
    EXPECT_NO_ERROR(checkSolution(maze, sol));
}
