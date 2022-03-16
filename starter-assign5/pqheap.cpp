#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "set.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;

/*
 * This is the constructor for the PQHeap class.
 * It sets the number of filled elements to 0, the number allocated
 * to the INITIAL_CAPACITY, and allocates memory for the _elements.
 */
PQHeap::PQHeap() {
    _numFilled = 0;
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[INITIAL_CAPACITY];
}

/*
 * This is the deconstructor for the PQHeap class.
 * It clears the _elements allocated heap memory to avoid
 * issues.
 */
PQHeap::~PQHeap() {
    delete[] _elements;
}

/* This is a helper function that
 * doubles the size of the allocated array
 * and copys all previous elements.
 */
void PQHeap::expand() {
    _numAllocated *= 2;
    DataPoint* expanded = new DataPoint[_numAllocated];
    for (int pos = 0; pos < _numFilled; pos++) {
        expanded[pos] = _elements[pos];
    }
    delete[] _elements;
    _elements = expanded;
}

/* This is a helper function that swaps
 * two elements in two different positions.
 */
void PQHeap::swap(int firstIndex, int secondIndex) {
    DataPoint tmp = _elements[firstIndex];
    _elements[firstIndex] = _elements[secondIndex];
    _elements[secondIndex] = tmp;
}

/* This is a helper function for the `enqueue` method
 * that bubblesUp to rearrange any binary heap nodes
 * out of order.
 */
void PQHeap::bubbleUp(int index) {
    int parent = getParentIndex(index);
    if (_elements[parent].priority < _elements[index].priority || index == 0) {
        return;
    } else {
        swap(index, parent);
        bubbleUp(parent);
    }
}

/* This is a helper function for the `dequeue` method that
 * bubblesDown to rearrange any binary heap nodes
 * out of order.
 */
void PQHeap::bubbleDown(int index) {
    if (index > _numFilled - 1) return;

    int left = getLeftChildIndex(index), right = getRightChildIndex(index);
    bool leftSmaller = _elements[left].priority < _elements[index].priority, rightSmaller = _elements[right].priority < _elements[index].priority;

    if (leftSmaller && rightSmaller) {
        int lowerChildIndex = _elements[left].priority < _elements[right].priority ? left : right;
        swap(lowerChildIndex, index);
        bubbleDown(lowerChildIndex);
    } else if (leftSmaller) {
        swap(left, index);
        bubbleDown(left);
    } else if (rightSmaller) {
        swap(right, index);
        bubbleDown(right);
    }
}

/*
 * This is the function to enqueue a DataPoint into the
 * priority queue.
 */
void PQHeap::enqueue(DataPoint elem) {
    if (_numFilled + 1 > _numAllocated) {
        expand();
    }
    _elements[_numFilled] = elem;
    bubbleUp(_numFilled);
    _numFilled++;
}

/*
 * This method returns the next element in the queue without
 * removing it.
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("Cannot peek an empty queue.");
    } else {
        return _elements[0];
    }
}

/*
 * This method removes the next element in the queue and
 * returns it.
 */
DataPoint PQHeap::dequeue() {
    if (isEmpty()) {
        error("Cannot dequeue an empty pqueue.");
    }

    DataPoint removed = _elements[0];
    _elements[0] = _elements[_numFilled - 1];
    _numFilled--;
    bubbleDown(0);
    return removed;
}

/*
 * This method returns a boolean true if the queue is empty
 * and false otherwise.
 */
bool PQHeap::isEmpty() const {
    return _numFilled == 0;
}

/*
 * This method returns the size of the number of
 * elements currently tracked in the array.
 */
int PQHeap::size() const {
    return _numFilled;
}

/*
 * This method clears the queue and reinitializes most
 * of the member variables to the originals.
 */
void PQHeap::clear() {
    DataPoint* newShell = new DataPoint[INITIAL_CAPACITY];
    _elements = newShell;
    _numAllocated = INITIAL_CAPACITY;
    _numFilled = 0;
}

/*
 * This method prints out the elements in the queue
 * for debugging purposes.
 */
void PQHeap::printDebugInfo() {
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << _elements[i] << endl;
    }
}

/* This is a helper function for
 * `validateInternalState` that makes sure a node
 * has valid children
 */
void PQHeap::checkValidChildren(int index, Set<int> &visited) {
    if (visited.contains(index)) return;

    visited.add(index);

    if (index != 0 && (_elements[getParentIndex(index)].name != "" ||  _elements[index].name != "") && _elements[getParentIndex(index)].priority > _elements[index].priority) {
//        printDebugInfo();
//        cout << "parent: " << getParentIndex(index) << " current index: " << index << endl;
        error("Array elements out of order at index " + integerToString(index));
    } else {
         int left = getLeftChildIndex(index), right = getLeftChildIndex(index);
         if (left < _numFilled) {
             checkValidChildren(left, visited);
         }
         if (right < _numFilled) {
             checkValidChildren(right, visited);
         }
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * should traverse the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */
void PQHeap::validateInternalState() {
    /*
     * If there are more elements than spots in the array, we have a problem.
     */
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");

    //Check to make sure all parents are less than children
    Set<int> visited;
    checkValidChildren(0, visited);
}

/*
 * We strongly recommend implementing this helper function, which
 * should calculate the index of the parent of the element with the
 * provided index.
 */
int PQHeap::getParentIndex(int curIndex) {
    return (curIndex - 1) / 2;
}

/*
 * We strongly recommend implementing this helper function, which
 * should calculate the index of the left child of the element with the
 * provided index.
 */
int PQHeap::getLeftChildIndex(int curIndex) {
    return 2 * curIndex + 1;
}

/*
 * We strongly recommend implementing this helper function, which
 * should calculate the index of the right child of the element with the
 * provided index.
 */
int PQHeap::getRightChildIndex(int curIndex) {
    return 2 * curIndex + 2;
}

/* * * * * * Test Cases Below This Point * * * * * */

PROVIDED_TEST("enqueue") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();

    for (auto dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }

    EXPECT_EQUAL(pq.size(), 9);
}

PROVIDED_TEST("size") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();

    for (auto dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }

    EXPECT_EQUAL(pq.size(), 9);
}

PROVIDED_TEST("isEmpty") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();

    for (auto dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }

    EXPECT(!pq.isEmpty());

    pq.clear();
    EXPECT(pq.isEmpty());
}

PROVIDED_TEST("dequeue") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();

    for (auto dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }

    while (!pq.isEmpty()) {
        pq.dequeue();
    }

    EXPECT_EQUAL(pq.size(), 0);
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();

    for (auto dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();
    }

    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}

static void fillQueue(PQHeap& pq, int n) {
    pq.clear(); // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({ "", i });
    }
}

static void emptyQueue(PQHeap& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

PROVIDED_TEST("PQHeap timing test, fillQueue and emptyQueue") {
    PQHeap pq;

    TIME_OPERATION(40000, fillQueue(pq, 40000));
    TIME_OPERATION(40000, emptyQueue(pq, 40000));
}

