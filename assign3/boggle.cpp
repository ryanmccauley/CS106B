/*
 * This file contains all of the methods that
 * protain to the boggle game.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"
using namespace std;

/* This function returns the amount of
 * points a given word returns.
 */
int points(string str) {
    if (str.length() <= 3) {
        return 0;
    } else {
        return str.length() - 3;
    }
}

/* This functions gets and returns a GridLocation's
 * neighbors on the board.
 */
Set<GridLocation> getNeighbors(Grid<char> &board, GridLocation place) {
    Set<GridLocation> possible = {
        { place.row - 1, place.col },
        { place.row + 1, place.col },
        { place.row, place.col + 1 },
        { place.row, place.col - 1 },
        { place.row - 1, place.col -1 },
        { place.row + 1, place.col + 1},
        { place.row - 1, place.col + 1 },
        { place.row + 1, place.col - 1 },
    };

    Queue<GridLocation> toRemove;

    for (GridLocation loc : possible) {
        if (!board.inBounds(loc)) {
            toRemove.enqueue(loc);
        }
    }

    while (!toRemove.isEmpty()) {
        possible.remove(toRemove.dequeue());
    }

    return possible;
}

/* This is the helper function for
 * the `scoreBoard` function.
 */
void scoreBoardHelper(Lexicon &lex, Grid<char> &board, GridLocation currLoc, Vector<GridLocation> visited, Set<string> &found, int &totalPoints, string path) {
    if (!lex.containsPrefix(path)) return;

    visited.add(currLoc);

    if (path.length() >= 4 && lex.contains(path) && !found.contains(path)) {
        found.add(path);
        totalPoints += points(path);
    }

    for (GridLocation loc : getNeighbors(board, currLoc)) {
        if (!visited.contains(loc)) {
            scoreBoardHelper(lex, board, loc, visited, found, totalPoints, path + board[loc.row][loc.col]);
        }
    }
}

/* This function recursively solves a boggle board
 * and returns the score of the words.
 */
int scoreBoard(Grid<char>& board, Lexicon& lex) {
    int points = 0;

    Vector<GridLocation> visited;
    Set<string> found;

    string path = "";

    for (int row = 0; row < board.numRows(); row++) {
        for (int col = 0; col < board.numCols(); col++) {
            GridLocation loc = { row, col };
            scoreBoardHelper(lex, board, loc, visited, found, points, path);
        }
    }

    return points;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}
