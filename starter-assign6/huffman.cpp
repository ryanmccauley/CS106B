#include "bits.h"
#include "treenode.h"
#include "huffman.h"
#include "map.h"
#include "vector.h"
#include "priorityqueue.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
using namespace std;

/* This is a helper function to print out
 * information regarding an EncodingTreeNode
 */
void print(EncodingTreeNode* node) {
    if (node == nullptr) {
        cout << "nullptr" << endl;
    } else {
        cout << "Node: isLeaf(" << (node->isLeaf() ? "true" : "false") << ") Char(" << (node->isLeaf() ? node->getChar() : ' ') << ")" << endl;
    }
}

/**
 * Given a Queue<Bit> containing the compressed message bits and the encoding tree
 * used to encode those bits, decode the bits back to the original message text.
 *
 * You can assume that tree is a well-formed non-empty encoding tree and
 * messageBits queue contains a valid sequence of encoded bits.
 */
string decodeText(EncodingTreeNode* tree, Queue<Bit>& messageBits) {
    string text = "";
    while (!messageBits.isEmpty()) {
        EncodingTreeNode* curr = tree;
        while (!curr->isLeaf()) {
            Bit bit = messageBits.dequeue();
            if (bit == 0) {
                curr = curr->zero;
            } else {
                curr = curr->one;
            }
        }
        text += curr->getChar();
    }
    return text;
}

/* This is a helper function for the `unflattenTree` method
 * that explores a tree recursively and unflattens it.
 */
EncodingTreeNode* unflattenHelper(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    if (treeShape.isEmpty()) {
        return nullptr;
    } else {
        Bit next = treeShape.dequeue();
        if (next == 0) {
            return new EncodingTreeNode(treeLeaves.dequeue());
        } else {
            EncodingTreeNode* fork = new EncodingTreeNode(nullptr, nullptr);

            fork->zero = unflattenHelper(treeShape, treeLeaves);
            fork->one = unflattenHelper(treeShape, treeLeaves);

            return fork;
        }
    }
}

/**
 * Reconstruct encoding tree from flattened form Queue<Bit> and Queue<char>.
 *
 * You can assume that the queues are well-formed and represent
 * a valid encoding tree.
 */
EncodingTreeNode* unflattenTree(Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    if (treeShape.isEmpty()) {
        return nullptr;
    } else {
        EncodingTreeNode* root = new EncodingTreeNode(nullptr, nullptr);

        if (treeShape.dequeue() == 0) {
            return root;
        } else {
            root->zero = unflattenHelper(treeShape, treeLeaves);
            root->one = unflattenHelper(treeShape, treeLeaves);

            return root;
        }
    }
}

/**
 * Decompress the given EncodedData and return the original text.
 *
 * You can assume the input data is well-formed and was created by a correct
 * implementation of compress.
 *
 * Your implementation may change the data parameter however you like. There
 * are no requirements about what it should look like after this function
 * returns.
 */
string decompress(EncodedData& data) {
    EncodingTreeNode* unflattened = unflattenTree(data.treeShape, data.treeLeaves);
    string decoded = decodeText(unflattened, data.messageBits);
    deallocateTree(unflattened);
    return decoded;
}

/* This is a helper function for the `buildHuffmanTree` method
 * that returns a Map of each characters relative occurrences in the
 * text string.
 */
Map<char, int> getOccurrences(string text) {
    Map<char, int> m;

    for (char letter : text) {
        m[letter]++;
    }

    return m;
}

/* This is a helper function for the `buildHuffmanTree` method that
 * finds the smallest two nodes based off of their weights in a forest.
 */
Vector<EncodingTreeNode*> findTwoSmallest(Vector<EncodingTreeNode*> &forest, Map<EncodingTreeNode*, int> &weights) {
    EncodingTreeNode* smallest = forest[0];
    EncodingTreeNode* secondSmallest = forest[1];

    for (EncodingTreeNode* node : forest) {
        if (weights[node] < weights[smallest]) {
            secondSmallest = smallest;
            smallest = node;
        } else if (weights[node] < weights[secondSmallest] && weights[node] > weights[smallest]) {
            secondSmallest = node;
        }
    }

    return { smallest, secondSmallest };
}

/**
 * Constructs an optimal Huffman coding tree for the given text, using
 * the algorithm described in lecture.
 *
 * Reports an error if the input text does not contain at least
 * two distinct characters.
 *
 * When assembling larger trees out of smaller ones, make sure to set the first
 * tree dequeued from the queue to be the zero subtree of the new tree and the
 * second tree as the one subtree.
 *
 */
EncodingTreeNode* buildHuffmanTree(string text) {
    Map<char, int> occurrences = getOccurrences(text);
    Map<EncodingTreeNode*, int> weights;

    for (char key : occurrences) {
        EncodingTreeNode* node = new EncodingTreeNode(key);
        weights[node] = occurrences[key];
    }

    Vector<EncodingTreeNode*> keys = weights.keys();

    while (keys.size() > 1) {
        Vector<EncodingTreeNode*> smallest = findTwoSmallest(keys, weights);
        EncodingTreeNode* subtree = new EncodingTreeNode(smallest[0], smallest[1]);
        weights[subtree] = weights[smallest[0]] + weights[smallest[1]];
        keys.add(subtree);

        keys.remove(keys.indexOf(smallest[0]));
        keys.remove(keys.indexOf(smallest[1]));
    }

    return keys[0];
}

/* This is a helper function for `exploreBitSequence` that
 * converts a string representation of a bit sequence
 * into a queue.
 */
Queue<Bit> stringBitSequenceToQueue(string sequence) {
    Queue<Bit> q;

    while (sequence.length() > 0) {
        q.enqueue(charToInteger(sequence[0]));

        sequence = sequence.substr(1);
    }

    return q;
}

/* This is a helper function for `encodeText` that creates a bit sequence
 * recursively.
 */
void exploreBitSequence(EncodingTreeNode* tree, string sequence, Map<char, Queue<Bit>> &m) {
    if (tree == nullptr) {
        return;
    } else {
        if (tree->isLeaf()) {
            m[tree->getChar()] = stringBitSequenceToQueue(sequence);
        } else {
            exploreBitSequence(tree->zero, sequence + '0', m);
            exploreBitSequence(tree->one, sequence + '1', m);
        }
    }
}

/**
 * Given a string and an encoding tree, encode the text using the tree
 * and return a Queue<Bit> of the encoded bit sequence.
 *
 * You can assume tree is a valid non-empty encoding tree and contains an
 * encoding for every character in the text.
 */
Queue<Bit> encodeText(EncodingTreeNode* tree, string text) {    
    Map<char, Queue<Bit>> m;
    Queue<Bit> encoded;

    exploreBitSequence(tree, "", m);

    for (char letter : text) {
        Queue<Bit> bitSequence = m[letter];

        while (!bitSequence.isEmpty()) {
            encoded.enqueue(bitSequence.dequeue());
        }
    }

    return encoded;
}

/**
 * Flatten the given tree into a Queue<Bit> and Queue<char> in the manner
 * specified in the assignment writeup.
 *
 * You can assume the input queues are empty on entry to this function.
 *
 * You can assume tree is a valid well-formed encoding tree.
 *
 */
void flattenTree(EncodingTreeNode* tree, Queue<Bit>& treeShape, Queue<char>& treeLeaves) {
    if (tree->isLeaf()) {
        treeShape.enqueue(0);
        treeLeaves.enqueue(tree->getChar());
    } else {
        treeShape.enqueue(1);
        flattenTree(tree->zero, treeShape, treeLeaves);
        flattenTree(tree->one, treeShape, treeLeaves);
    }
}

/**
 * Compress the input text using Huffman coding, producing as output
 * an EncodedData containing the encoded message and flattened
 * encoding tree used.
 *
 * Reports an error if the message text does not contain at least
 * two distinct characters.
 *
 */
EncodedData compress(string messageText) {
    EncodedData data;

    EncodingTreeNode* tree = buildHuffmanTree(messageText);

    data.messageBits = encodeText(tree, messageText);

    flattenTree(tree, data.treeShape, data.treeLeaves);

    deallocateTree(tree);

    return data;
}

/* * * * * * Testing Helper Functions Below This Point * * * * * */

EncodingTreeNode* createExampleTree() { 
    /* Example encoding tree used in multiple test cases:
     *                *
     *              /   \
     *             T     *
     *                  / \
     *                 *   E
     *                / \
     *               R   S
     */
    EncodingTreeNode* R = new EncodingTreeNode('R');
    EncodingTreeNode* S = new EncodingTreeNode('S');
    EncodingTreeNode* E = new EncodingTreeNode('E');
    EncodingTreeNode* T = new EncodingTreeNode('T');
    EncodingTreeNode* bottomSplit = new EncodingTreeNode(R, S);
    EncodingTreeNode* middleSplit = new EncodingTreeNode(bottomSplit, E);
    EncodingTreeNode* root = new EncodingTreeNode(T, middleSplit);
    return root;
}

void deallocateTree(EncodingTreeNode* t) {
    if (t == nullptr) {
        return;
    } else {
        if (!t->isLeaf()) {
            deallocateTree(t->zero);
            deallocateTree(t->one);
        }
        delete t;
    }
}

bool areEqual(EncodingTreeNode* a, EncodingTreeNode* b) {
    if (a == nullptr || b == nullptr) {
        return a == b;
    } else if (a->isLeaf() || b->isLeaf()) {
        return a->isLeaf() && b->isLeaf() && a->getChar() == b->getChar();
    } else {
        return areEqual(a->zero, b->zero) && areEqual(a->one, b->one);
    }
}

/* * * * * * Test Cases Below This Point * * * * * */

STUDENT_TEST("deallocateTree test cases") {
    EncodingTreeNode* tree = createExampleTree();
    deallocateTree(tree);
}

STUDENT_TEST("areEqual test cases") {
    EncodingTreeNode* singleton = new EncodingTreeNode('A');
    EncodingTreeNode* empty = nullptr;

    EXPECT(!areEqual(singleton, empty));

    EncodingTreeNode* leafRightChar = new EncodingTreeNode('A');
    EncodingTreeNode* leafWrongChar = new EncodingTreeNode('B');

    EXPECT(areEqual(singleton, leafRightChar));

    EXPECT(!areEqual(singleton, leafWrongChar));

    EncodingTreeNode* exampleTree = createExampleTree();

    EXPECT(!areEqual(singleton, exampleTree));

    EncodingTreeNode* anotherExampleTree = createExampleTree();
    EXPECT(areEqual(anotherExampleTree, exampleTree));

    EXPECT(!areEqual(exampleTree, exampleTree->zero));

    deallocateTree(singleton);
    deallocateTree(leafRightChar);
    deallocateTree(leafWrongChar);
    deallocateTree(exampleTree);
    deallocateTree(anotherExampleTree);
}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("decodeText, small example encoding tree") {
    EncodingTreeNode* tree = createExampleTree(); // see diagram above
    EXPECT(tree != nullptr);

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(decodeText(tree, messageBits), "E");

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(decodeText(tree, messageBits), "SET");

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1}; // STREETS
    EXPECT_EQUAL(decodeText(tree, messageBits), "STREETS");

    deallocateTree(tree);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("unflattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  treeShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeLeaves = { 'T', 'R', 'S', 'E' };
    EncodingTreeNode* tree = unflattenTree(treeShape, treeLeaves);

    EXPECT(areEqual(tree, reference));

    deallocateTree(tree);
    deallocateTree(reference);
}

PROVIDED_TEST("decompress, small example input") {
    EncodedData data = {
        { 1, 0, 1, 1, 0, 0, 0 }, // treeShape
        { 'T', 'R', 'S', 'E' },  // treeLeaves
        { 0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1 } // messageBits
    };

    EXPECT_EQUAL(decompress(data), "TRESS");
}

PROVIDED_TEST("buildHuffmanTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    EncodingTreeNode* tree = buildHuffmanTree("STREETTEST");

    // Huffman tree has a different shape & leaves order
    EXPECT(areEqual(tree, reference));

    deallocateTree(reference);
    deallocateTree(tree);
}

PROVIDED_TEST("encodeText, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above

    Queue<Bit> messageBits = { 1, 1 }; // E
    EXPECT_EQUAL(encodeText(reference, "E"), messageBits);

    messageBits = { 1, 0, 1, 1, 1, 0 }; // SET
    EXPECT_EQUAL(encodeText(reference, "SET"), messageBits);

    messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1 }; // STREETS
    EXPECT_EQUAL(encodeText(reference, "STREETS"), messageBits);

    deallocateTree(reference);
}

PROVIDED_TEST("flattenTree, small example encoding tree") {
    EncodingTreeNode* reference = createExampleTree(); // see diagram above
    Queue<Bit>  expectedShape  = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> expectedLeaves = { 'T', 'R', 'S', 'E' };

    Queue<Bit>  treeShape;
    Queue<char> treeLeaves;
    flattenTree(reference, treeShape, treeLeaves);

    EXPECT_EQUAL(treeShape,  expectedShape);
    EXPECT_EQUAL(treeLeaves, expectedLeaves);

    deallocateTree(reference);
}

PROVIDED_TEST("compress, small example input") {
    EncodedData data = compress("STREETTEST");
    Queue<Bit>  treeShape   = { 1, 0, 1, 1, 0, 0, 0 };
    Queue<char> treeChars   = { 'T', 'R', 'S', 'E' };
    Queue<Bit>  messageBits = { 1, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0 };

    // Huffman tree has a different shape & leaves order
    EXPECT_EQUAL(data.treeShape, treeShape);
    EXPECT_EQUAL(data.treeLeaves, treeChars);
    EXPECT_EQUAL(data.messageBits, messageBits);
}

PROVIDED_TEST("Test end-to-end compress -> decompress") {
    Vector<string> inputs = {
        "HAPPY HIP HOP",
        "Nana Nana Nana Nana Nana Nana Nana Nana Batman"
        "Research is formalized curiosity. It is poking and prying with a purpose. – Zora Neale Hurston",
    };

    for (string input: inputs) {
        EncodedData data = compress(input);
        string output = decompress(data);

        EXPECT_EQUAL(input, output);
    }
}
