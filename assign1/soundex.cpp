/*
 * This is the soundex.cpp file which is used for
 * finding the soundex of words and searching through a
 * database of words to match them.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function takes a string s and returns a version of
 * the string that has all non-letter characters removed
 * from it.
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
    for (int i = 0; i < s.length(); i++) {
        if (!isalpha(s[i])) {
            // remove the character at index i
            s = s.substr(0, i) + s.substr(i + 1);
            i--;
        }

    }
    return s;
}

/* This function takes a string and replaces the digits
 * with a certain number value corresponding with the sounds.
 */
string encodeWithDigits(string s) {
    for (int i = 0; i < s.length(); i++) {
        char c = toupper(s[i]);
        if (c == 'R') {
            s[i] = '6';
        } else if (c == 'M' || c == 'N') {
            s[i] = '5';
        } else if (c == 'L') {
            s[i] = '4';
        } else if (c == 'D' || c == 'T') {
            s[i] = '3';
        } else if (c == 'C' || c == 'G' || c == 'J' || c == 'K' || c == 'Q' || c == 'S' || c == 'X' || c == 'Z') {
            s[i] = '2';
        } else if (c == 'B' || c == 'F' || c == 'P' || c == 'V') {
            s[i] = '1';
        } else {
            s[i] = '0';
        }
    }
    return s;
}

/* This function removes duplicate digits from a string of digits.
 */
string removeDuplicates(string s) {
    for (int i = 1; i < s.length() - 1; i++) {
        if (s[i] == s[i + 1] || s[i] == s[i - 1]) {
            s = s.substr(0, i) + s.substr(i + 1);
            i--;
        }
    }
    return s;
}

/* This function replaces the first digit in an encoded string
 * with the first character of the original string.
 */
string replaceFirst(string original, string s) {
    s[0] = toupper(original[0]);
    return s;
}

/* This function removes all zeros from a string of encoded
 * digits.
 */
string removeZeros(string s) {
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '0') {
            s = s.substr(0, i) + s.substr(i + 1);
            i--;
        }
    }
    return s;
}

/* This function makes the soundex string have a length of 4
 * by truncating the excess or adding zeros on the end.
 */
string makeLength(string s) {
    if (s.length() < 4) {
        while (s.length() < 4) {
            s += '0';
        }
        return s;
    } else {
        return s.substr(0, 4);
    }
}

/* The function takes a string named s and returns the
 * corresponding soundex string to that original string.
 */
string soundex(string s) {
    string nonLettersRemoved = removeNonLetters(s);
    string digitEncoded = encodeWithDigits(nonLettersRemoved);
    string duplicatesRemoved = removeDuplicates(digitEncoded);
    string firstReplaced = replaceFirst(s, duplicatesRemoved);
    string zerosRemoved = removeZeros(firstReplaced);
    string correctLength = makeLength(zerosRemoved);
    return correctLength;
}


/* This function creates a database of names from the specified
 * file and allows the user to input to try and see matching soundexs
 * of names in the database.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    bool asking = true;
    while (asking) {
        string input = getLine("Enter a surname (RETURN to quit): ");
        if (input == "") {
            asking = false;
            cout << "All done!";
        } else {
            string code = soundex(input);
            cout << "Soundex code is " << code << endl;
            Vector<string> matchingNames;
            for (string entry : databaseNames) {
                if (soundex(entry) == code) {
                    matchingNames += entry;
                }
            }
            matchingNames.sort();
            cout << "Matches from database: " << matchingNames << endl;
        }
    }
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("Curie"), "C600");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here
STUDENT_TEST("removeNonLetters bug test cases") {
    string s = "Ry_a--n";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Ryan");
    s = "";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
    s = "_______";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "");
}

STUDENT_TEST("encodeWithDigits test cases") {
    string s = "Ryan";
    string result = encodeWithDigits(s);
    EXPECT_EQUAL(result, "6005");
    s = "John";
    result = encodeWithDigits(s);
    EXPECT_EQUAL(result, "2005");
}

STUDENT_TEST("removeDuplicates test cases") {
    string s = "Ryaaaaaaaaan";
    string encoded = encodeWithDigits(s);
    string result = removeDuplicates(encoded);
    EXPECT_EQUAL(result, "605");
    s = "Johhhhhnnnn";
    encoded = encodeWithDigits(s);
    result = removeDuplicates(encoded);
    EXPECT_EQUAL(result, "205");
}

STUDENT_TEST("replaceFirst test cases") {
    string s = "ryan";
    string encoded = encodeWithDigits(s);
    string result = replaceFirst(s, encoded);
    EXPECT_EQUAL(result, "R005");
    s = "John";
    encoded = encodeWithDigits(s);
    result = replaceFirst(s, encoded);
    EXPECT_EQUAL(result, "J005");
}

STUDENT_TEST("removeZeros test cases") {
    string s = "Ryaaaaaaaaaaan";
    string encoded = encodeWithDigits(s);
    string result = removeZeros(encoded);
    EXPECT_EQUAL(result, "65");
    s = "Jooooooooooooohn";
    encoded = encodeWithDigits(s);
    result = removeZeros(encoded);
    EXPECT_EQUAL(result, "25");
}

STUDENT_TEST("makeLength test cases") {
    string s = "Ryaaaaaaaaaaan";
    string encoded = encodeWithDigits(s);
    string nonZeros = removeZeros(encoded);
    string result = makeLength(nonZeros);
    EXPECT_EQUAL(result, "6500");
    s = "Jooooooooooooohn";
    encoded = encodeWithDigits(s);
    nonZeros = removeZeros(encoded);
    result = makeLength(nonZeros);
    EXPECT_EQUAL(result, "2500");
}
