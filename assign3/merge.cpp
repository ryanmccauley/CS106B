/*
 * This file contains all of the methods that
 * protain to merging queues iteratively and recursively.
 */
#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

/* This helper functions returns whether or not
 * a queue is sorted.
 */
bool isSorted(Queue<int> q) {
    Vector<int> v;

    while (!q.isEmpty()) {
        v.add(q.dequeue());
    }

    for (int i = 0; i < v.size() - 1; i++) {
        if (v[i] > v[i + 1]) return false;
    }

    return true;
}

/* This functions merges two sorted queues
 * without iteratively.
 */
Queue<int> merge(Queue<int> a, Queue<int> b) {
    Queue<int> result;

    if (!isSorted(a) || !isSorted(b)) {
        error("Input queues not sorted.");
    }

    // Must store before while loop because a & b sizes will change as iteration goes on.
    int finalSize = a.size() + b.size();

    while (result.size() < finalSize) {
        if (a.isEmpty() && !b.isEmpty()) {
            result.enqueue(b.dequeue());
        } else if (!a.isEmpty() && b.isEmpty()) {
            result.enqueue(a.dequeue());
        } else {
            int nextA = a.peek();
            int nextB = b.peek();

            if (nextA < nextB) {
                result.enqueue(a.dequeue());
            } else {
                result.enqueue(b.dequeue());
            }
        }
    }

    return result;
}

/*
 * This function assumes correct functionality of the previously
 * defined merge function and makes use of this function to
 * iteratively merge a collection of sequences, starting off with
 * an empty sequence and progressively merging in one sequence at
 * a time. This function is provided for you fully implemented –
 * it does not need to modified at all.
 */
Queue<int> multiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = merge(q, result);
    }
    return result;
}

/* This function recursively merges multiple
 * sorted queues into one sorted queue.
 */
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    if (all.size() == 1) {
        return all[0];
    } else {
        Queue<int> result;

        // Handle even or odd output
        if (all.size() % 2 == 0) {
            Vector<Queue<int>> a = all.subList(0, all.size() / 2);
            Vector<Queue<int>> b = all.subList(all.size() / 2, all.size() / 2);

            Queue<int> left = recMultiMerge(a);
            Queue<int> right = recMultiMerge(b);

            result = merge(left, right);
            return result;
        } else {
            Vector<Queue<int>> a = all.subList(0, all.size() / 2);
            Vector<Queue<int>> b = all.subList(all.size() / 2, (all.size() / 2) + 1);

            Queue<int> left = recMultiMerge(a);
            Queue<int> right = recMultiMerge(b);

            result = merge(left, right);
            return result;
        }
    }
}


/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

PROVIDED_TEST("Test binary merge, two short sequences") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(merge(a, b), expected);
    EXPECT_EQUAL(merge(b, a), expected);
}

PROVIDED_TEST("Test multiMerge, small collection of short sequences") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(multiMerge(all), expected);
}

PROVIDED_TEST("Test recMultiMerge by compare to multiMerge") {
    int n = 1000;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(n);
    distribute(input, all);

    EXPECT_EQUAL(multiMerge(all), recMultiMerge(all));
}

PROVIDED_TEST("Time binary merge operation") {
    int n = 1500000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);
    TIME_OPERATION(a.size(), merge(a, b));
}

PROVIDED_TEST("Time multiMerge operation") {
    int n = 11000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), multiMerge(all));
}
PROVIDED_TEST("Time recMultiMerge operation") {
    int n = 90000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("multiMerge 0 queues, or many empty queues.") {
    Vector<Queue<int>> v;
    Queue<int> q;
    EXPECT_EQUAL(multiMerge(v), q);

    v = { {}, {} };
    EXPECT_EQUAL(multiMerge(v), q);
}

STUDENT_TEST("Time operation merge 1") {
    int n = 100000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);

    TIME_OPERATION(n * 2, merge(a, b));
}

STUDENT_TEST("Time operation merge 2") {
    int n = 200000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);

    TIME_OPERATION(n * 2, merge(a, b));
}

STUDENT_TEST("Time operation merge 3") {
    int n = 300000;
    Queue<int> a = createSequence(n);
    Queue<int> b = createSequence(n);

    TIME_OPERATION(n * 2, merge(a, b));
}

STUDENT_TEST("Time operation multiMerge 1") {
    int n = 12500;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);

    TIME_OPERATION(input.size(), multiMerge(all));
}

STUDENT_TEST("Time operation multiMerge 2") {
    int n = 25000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);

    TIME_OPERATION(input.size(), multiMerge(all));
}

STUDENT_TEST("Time operation multiMerge 3") {
    int n = 50000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);

    TIME_OPERATION(input.size(), multiMerge(all));
}

STUDENT_TEST("Time operation recMultiMerge 1") {
    int n = 10000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("Time operation recMultiMerge 2") {
    int n = 20000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}

STUDENT_TEST("Time operation recMultiMerge 3") {
    int n = 40000;
    int k = n/10;
    Queue<int> input = createSequence(n);
    Vector<Queue<int>> all(k);
    distribute(input, all);
    TIME_OPERATION(input.size(), recMultiMerge(all));
}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}
