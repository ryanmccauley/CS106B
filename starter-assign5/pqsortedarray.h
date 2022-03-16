#pragma once
#include "testing/MemoryUtils.h"
#include "datapoint.h"

/**
 * Priority queue of DataPoints implemented using a sorted array.
 */
class PQSortedArray {
public:
    /**
     * Creates a new, empty priority queue.
     */
    PQSortedArray();

    /**
     * Cleans up all memory allocated by this priority queue.
     */
    ~PQSortedArray();

    /**
     * Adds a new element into the queue. This operation runs in time O(N),
     * where n is the number of elements in the queue.
     *
     * @param element The element to add.
     */
    void enqueue(DataPoint element);

    /**
     * Removes and returns the element that is frontmost in this priority queue.
     * The frontmost element is the one with lowest priority value.
     *
     * If the priority queue is empty, this function calls error().
     *
     * This operation runs in time O(1).
     *
     * @return The frontmost element, which is removed from queue.
     */
    DataPoint dequeue();

    /**
     * Returns, but does not remove, the element that is frontmost.
     *
     * If the priority queue is empty, this function calls error().
     *
     * This operation runs in time O(1).
     *
     * @return frontmost element
     */
    DataPoint peek() const;

    /**
     * Returns whether this priority queue is empty.
     *
     * This operation runs in time O(1).
     *
     * @return true if contains no elements, false otherwise.
     */
    bool isEmpty() const;

    /**
     * Returns the count of elements in this priority queue.
     *
     * This operation runs in time O(1).
     *
     * @return The count of elements in the priority queue.
     */
    int size() const;

    /**
     * Removes all elements from the priority queue.
     *
     * This operation runs in time O(1).
     */
    void clear();

    /**
     * This function exists purely for testing purposes. You can have it do
     * whatever you'd like. We will not invoke it when grading.
     *
     * The provided implementation prints out the contents of the array
     * for easy viewing purposes.
     */
    void printDebugInfo(std::string label);

    /*
     * This function exits purely for testing purposes. It verifies
     * that the internal state of the queue is valid/consistent.
     * If a problem is detected, this function calls error().
     * If no problem, the function returns normally.
     */
    void validateInternalState();

private:
    DataPoint* _elements;   // dynamic array
    int _numAllocated;      // number of slots allocated in array
    int _numFilled;         // number of slots filled in array

    void expand();
    int findPositionToInsert(DataPoint elem);

    /* Weird C++isms: C++ loves to make copies of things, which is usually a good thing but
     * for the purposes of this assignment requires some C++ knowledge we haven't yet covered.
     * This next line disables all copy functions to make sure you don't accidentally end up
     * debugging something that isn't your fault.
     *
     * Curious what this does? Take CS106L!
     */
    DISALLOW_COPYING_OF(PQSortedArray);
};
