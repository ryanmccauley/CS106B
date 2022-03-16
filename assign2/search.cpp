/* This file is used for creating a search for the files in the res/ folder.
 * Included are the cleanToken, gatherTokens, buildIndex, searchEngine, & findQueryMatches methods.
 */
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "search.h"
#include "set.h"
#include "strlib.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* This function is used for "cleaning" a token where it trims it,
 * converts it to lowercase, & returns empty if there is no alphabetic
 * characters.
 */
string cleanToken(string s)
{
    // Check if string does not contain at least one letter
    bool containsAlpha = false;
    for (char character : s) {
        if (isalpha(character)) {
            containsAlpha = true;
            break;
        }
    }
    if (!containsAlpha) {
        return "";
    }

    // Remove punctuation from beginning and ending
    int index = 0;

    while (ispunct(s[0])) {
        s = s.substr(1);
    }

    while (ispunct(s[s.length() - 1])) {
        s = s.substr(0, s.length() - 1);
    }

    // Convert all to lowercase
    return toLowerCase(s);
}

/* This function is used for gathering the cleaned tokens
 * separated by a " " from a given body of text.
 */
Set<string> gatherTokens(string text)
{
    Set<string> tokens;

    // Tokenized body text
    Vector<string> tokenized = stringSplit(text, " ");

    for (string token : tokenized) {
        string cleaned = cleanToken(token);
        if (cleaned != "") {
            tokens.add(cleaned);
        }
    }

    return tokens;
}

/* This function creates an inverted index from a file of
 * urls and body elements.
 */
int buildIndex(string dbfile, Map<string, Set<string>>& index)
{
    int docsProcessed = 0;

    ifstream in;

    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);

    Vector<string> lines;
    readEntireFile(in, lines);

    for (int i = 0; i < lines.size(); i += 2) {
        string url = lines[i];
        string body = lines[i + 1];

        Set<string> tokens = gatherTokens(body);
        for (string token : tokens) {
            index[token].add(url);
        }
        docsProcessed++;
    }

    return docsProcessed;
}

/* This function returns the matches of a simple or complex
 * query based on rules and from a supplied inverted index.
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string sentence)
{
    Set<string> result;

    // Handle queries no matter the character case
    string lowerCaseQuery = "";
    for (char character : sentence) {
        lowerCaseQuery += character;
    }

    Vector<string> queryTerms = stringSplit(lowerCaseQuery, " ");

    // clean queries
    for (string& s : queryTerms) {
        // Don't remove +/- from trimming
        if (startsWith(s, '+')) {
            s = '+' + cleanToken(s.substr(1));
        } else if (startsWith(s, '-')) {
            s = '-' + cleanToken(s.substr(1));
        } else {
            s = cleanToken(s);
        }
    }

    for (string query : queryTerms) {
        if (startsWith(query, '+')) {
            if (index.containsKey(cleanToken(query))) {
               result = result.intersect(index[cleanToken(query)]);
            }
        } else if (startsWith(query, '-')) {
            if (index.containsKey(cleanToken(query))) {
                for (string url : index[cleanToken(query)]) {
                    result.remove(url);
                }
            }
        } else {
            if (index.containsKey(query)) {
                for (string url : index[query]) {
                    result.add(url);
                }
            }
        }
    }

    return result;
}

/* This function is the console client that the user
 * can use to create a search engine from a file and search
 * it using the console.
 */
void searchEngine(string dbfile)
{
    Map<string, Set<string>> index;
    int processed = buildIndex(dbfile, index);
    int distinct = index.size();
    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << processed << " pages containing " << distinct << " unique terms" << endl;

    while (true) {
        cout << endl << "Enter query sentence (RETURN/ENTER to quit): ";
        string query;
        getline(cin, query);
        if (query == "") {
            break;
        } else {
            Set<string> matches = findQueryMatches(index, query);
            cout << "Found " << matches.size() << " matching pages" << endl;
            cout << matches << endl;
        }
    }
}

/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on tokens with no punctuation") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
}

PROVIDED_TEST("cleanToken on tokens with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word tokens"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> tokens = gatherTokens("to be or not to be");
    EXPECT_EQUAL(tokens.size(), 4);
}

PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> seuss = gatherTokens("One Fish Two Fish Red fish Blue fish!!! 123");
    EXPECT_EQUAL(seuss.size(), 5);
    EXPECT(seuss.contains("fish"));
    EXPECT(!seuss.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 20 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 20);
    EXPECT(index.containsKey("fish"));
    EXPECT(!index.containsKey("@"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 3);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}

STUDENT_TEST("`cleanToken` test cases") {

    // Non alpha string
    string s = "-_-_-_-_-\~";
    EXPECT_EQUAL(cleanToken(s), "");

    // Empty string
    s = "";
    EXPECT_EQUAL(cleanToken(s), "");

    // Lowercase string
    s = "RYAN";
    EXPECT_EQUAL(cleanToken(s), "ryan");

    // Trim string
    s = "\"ryan\"";
    EXPECT_EQUAL(cleanToken(s), "ryan");
}

STUDENT_TEST("`gatherTokens` test cases") {
    Set<string> tokens = gatherTokens("First Second Third Third Fourth_ FIFTH");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fifth"));
    EXPECT(!tokens.contains("Fourth"));

    tokens = gatherTokens("A A A A B B B B C C C C D D d d E E e e f f f f_");
    EXPECT_EQUAL(tokens.size(), 6);
    EXPECT(tokens.contains("a"));
    EXPECT(!tokens.contains("f_"));

    tokens = gatherTokens("gather TOKENS from ->this _message");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("gather"));
    EXPECT(!tokens.contains("->this"));
    EXPECT(!tokens.contains("_message"));
}

STUDENT_TEST("buildIndex test cases") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 20);
    EXPECT(index.containsKey("milk"));
    EXPECT(!index.containsKey("!!!"));
    EXPECT(index.containsKey("bread"));
    EXPECT(index.values().contains({ "www.bigbadwolf.com" }));
}

STUDENT_TEST("findQueryMatches test cases") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesFish = findQueryMatches(index, "fish");
    EXPECT_EQUAL(matchesFish.size(), 2);
    Set<string> matchesShark = findQueryMatches(index, "shark");
    EXPECT(matchesShark.isEmpty());
    Set<string> matchesComplex = findQueryMatches(index, "fish -red milk");
    EXPECT_EQUAL(matchesComplex.size(), 1);
}
