/*
 * This file contains all of the methods that
 * protain to balancing operators using recursion
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

/* This is a helper function to
 * operatorsFrom to determine whether
 * a character is an operator or not.
 */
bool isOperator(char c) {
    return c == '(' || c == ')' || c == '{' || c == '}'
           || c == ']' || c == '[';
}

/* This function returns ths operators from
 * a string
 */
string operatorsFrom(string str) {
    if (str.length() == 0) {
        return "";
    } else if (isOperator(str[0])) {
        return str[0] + operatorsFrom(str.substr(1));
    } else {
        return operatorsFrom(str.substr(1));
    }
}

/* This is a helper method to operatorsAreMatched
 * to determine the first index of an occurence
 * of a pair of operators.
 */
int indexOfPair(string str) {
    return stringIndexOf(str, "()") != -1 ? stringIndexOf(str, "()") : stringIndexOf(str, "{}") != -1 ? stringIndexOf(str, "{}") :
            stringIndexOf(str, "[]");
}

/* This function returns whether or not a string has
 * matched operators.
 */
bool operatorsAreMatched(string ops) {
    if (ops.length() == 0) {
        return true;
    } else {
        if (indexOfPair(ops) != -1) {
            return operatorsAreMatched(ops.substr(0, indexOfPair(ops)) + ops.substr(indexOfPair(ops) + 2));
        } else {
            return false;
        }
    }
}

/*
 * The isBalanced function assumes correct implementation of
 * the above two functions operatorsFrom and operatorsMatch.
 * It uses operatorsFrom to extract the operator characters
 * from the input string and then confirms that those
 * operators are balanced by using operatorsMatch.
 * You should not modify the provided code in the isBalanced
 * function.  If the previous two functions have been implemented
 * correctly, the provided isBalanced will correctly report whether
 * the input string has balanced bracketing operators.
 */
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("operatorsFrom on simple example") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

PROVIDED_TEST("operatorsAreMatched on simple example") {
    EXPECT(operatorsAreMatched("{}"));
}

PROVIDED_TEST("isBalanced on example from writeup") {
    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
    EXPECT(isBalanced(example));
}

PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
    EXPECT(!isBalanced("( ( [ a ] )"));
    EXPECT(!isBalanced("3 ) ("));
    EXPECT(!isBalanced("{ ( x } y )"));
}

STUDENT_TEST("operatorsFrom tests") {
    EXPECT_EQUAL(operatorsFrom("(hello world)[]"), "()[]");
    EXPECT_EQUAL(operatorsFrom("[[[[[[]]]"), "[[[[[[]]]");
    EXPECT_EQUAL(operatorsFrom(""), "");
}

STUDENT_TEST("isBalanced tests") {
     EXPECT(isBalanced("(hello world)[]"));
     EXPECT(!isBalanced("[[[[[[]]]"));
     EXPECT(isBalanced(""));
     EXPECT(!isBalanced("["));
}
