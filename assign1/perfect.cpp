/*
 * This is the perfect.cpp file which is used
 * to find perfect numbers.
 */
#include "console.h"
#include <iostream>
#include "testing/SimpleTest.h"
using namespace std;

/* This function takes one argument `n` and calculates the sum
 * of all proper divisors of `n` excluding itself. To find divisors
 * a loop iterates over all numbers from 1 to n-1, testing for a
 * zero remainder from the division.
 *
 * Note: long is a C++ type is a variant of int that allows for a
 * larger range of values. For all intents and purposes, you can
 * treat it like you would an int.
 */
long divisorSum(long n) {
    long total = 0;
    for (long divisor = 1; divisor < n; divisor++) {
        if (n % divisor == 0) {
            total += divisor;
        }
    }
    return total;
}

/* This function takes one argument `n` and returns a boolean
 * (true/false) value indicating whether or not `n` is perfect.
 * A perfect number is a non-zero positive number whose sum
 * of its proper divisors is equal to itself.
 */
bool isPerfect(long n) {
    return (n != 0) && (n == divisorSum(n));
}

/* This function does an exhaustive search for perfect numbers.
 * It takes one argument `stop` and searches the range 1 to `stop`,
 * checking each number to see whether it is perfect and if so,
 * printing it to the console.
 */
void findPerfects(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfect(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* This function takes in a number n and returns the 'smarter'
 * sum of its digits. It goes from 2 to the square root of the
 * number and sees whether it is perfect or not.
 */
long smarterSum(long n) {
     // Initalize the total as 1 because every numbers factors includes 1 and itself so we don't have to start the for-loop at
     // 1, instead we can start it at 1
    int total = 1;
    for (long divisor = 2; divisor <= sqrt(n); divisor++) {
        if (n / sqrt(n) == divisor) {
            // Handle square root so we don't increment twice
            total += sqrt(n);
        } else if (n % divisor == 0){
            // Increment by 2 because any number where number % divisor equals 0, there exists another number multiplied by the divisor to equal the number.
            // So, we increment by the divisor and the other number.
            total += divisor;
            total += (n / divisor);
        }
    }
    return total;
}

/* This function performs a 'smarter' analysis of
 * whether a number n is perfect or not. It implements
 * the smarterSum method to be faster as it onlys iterates
 * until the numbers square root.
 */
bool isPerfectSmarter(long n) {
    return (!(n <= 0)) && (n == smarterSum(n));
}

/* This function returns the amount of perfect numbers
 * in between 1 and the number exclusive. It implements
 * the smarter algorithms for a better time performance.
 */
void findPerfectsSmarter(long stop) {
    for (long num = 1; num < stop; num++) {
        if (isPerfectSmarter(num)) {
            cout << "Found perfect number: " << num << endl;
        }
        if (num % 10000 == 0) cout << "." << flush; // progress bar
    }
    cout << endl << "Done searching up to " << stop << endl;
}

/* This function is a helper function to findNthPerfectEuclid
 * and it computes whether a number is prime or not.
 */
bool isPrime(long n) {
    if (n == 1) return false;
    for (long divisor = 2; divisor < sqrt(n); divisor++) {
        if (n % divisor == 0) return false;
    }
    return true;
}

/* This functions computes the Nth perfect euclid number.
 * It uses an algorithm that finds the Nth perfect number
 * and returns it when the found index is equal to N.
 */
long findNthPerfectEuclid(long n) {
    long k = 1;
    long totalFound = 0;
    long lastFound = -1;
    while (totalFound < n) {
        long m = pow(2, k) - 1;
        if (isPrime(m)) {
            long perfect = pow(2, k - 1) * (pow(2, k) - 1);
            lastFound = perfect;
            totalFound++;
        }
        k++;
    }
    return lastFound;
}


/* * * * * * Test Cases * * * * * */

/* Note: Do not add or remove any of the PROVIDED_TEST tests.
 * You should add your own STUDENT_TEST tests below the
 * provided tests.
 */

PROVIDED_TEST("Confirm divisorSum of small inputs") {
    EXPECT_EQUAL(divisorSum(1), 0);
    EXPECT_EQUAL(divisorSum(6), 6);
    EXPECT_EQUAL(divisorSum(12), 16);
}

PROVIDED_TEST("6 and 28 are perfect") {
    EXPECT(isPerfect(6));
    EXPECT(isPerfect(28));
}

PROVIDED_TEST("12 and 98765 are not perfect") {
    EXPECT(!isPerfect(12));
    EXPECT(!isPerfect(98765));
}

PROVIDED_TEST("Oddballs, zero, one, negative") {
    EXPECT(!isPerfect(0));
    EXPECT(!isPerfect(1));
    EXPECT(!isPerfect(-6));
}

PROVIDED_TEST("33550336 is perfect") {
    EXPECT(isPerfect(33550336));
}

PROVIDED_TEST("Time multiple trials of findPerfects on doubling input sizes") {
    cout << endl;
    TIME_OPERATION(20000, findPerfects(20000));
    TIME_OPERATION(40000, findPerfects(40000));
    TIME_OPERATION(80000, findPerfects(80000));
    TIME_OPERATION(160000, findPerfects(160000));
    TIME_OPERATION(320000, findPerfects(320000));
    //TIME_OPERATION(640000, findPerfects(640000));
}

// TODO: add your student test cases here
STUDENT_TEST("Test negative values for isPerfect") {
    EXPECT_EQUAL(isPerfect(-5), false);
    EXPECT_EQUAL(isPerfect(-15), false);
    EXPECT_EQUAL(isPerfect(-250), false);
    EXPECT_EQUAL(isPerfect(-7500), false);
}

STUDENT_TEST("Deliberately make test case expect something erroneous") {
    EXPECT_EQUAL(isPerfect(5), true);
}

STUDENT_TEST("Smarter sum test cases") {
    EXPECT_EQUAL(smarterSum(25), divisorSum(25));
    EXPECT_EQUAL(smarterSum(64), divisorSum(64));
    EXPECT_EQUAL(smarterSum(256), divisorSum(256));
    EXPECT_EQUAL(smarterSum(2), divisorSum(2));
    EXPECT_EQUAL(smarterSum(7), divisorSum(7));
}

STUDENT_TEST("Time multiple trials of findPerfectsSmarter on doubling input sizes") {
    cout << endl;
    TIME_OPERATION(20000, findPerfectsSmarter(20000));
    TIME_OPERATION(40000, findPerfectsSmarter(40000));
    TIME_OPERATION(80000, findPerfectsSmarter(80000));
    TIME_OPERATION(160000, findPerfectsSmarter(160000));
    TIME_OPERATION(320000, findPerfectsSmarter(320000));
    TIME_OPERATION(640000, findPerfectsSmarter(640000));
}

STUDENT_TEST("isPrime Test Cases") {
    EXPECT_EQUAL(isPrime(1), false);
    EXPECT_EQUAL(isPrime(2), true);
    EXPECT_EQUAL(isPrime(37), true);
    EXPECT_EQUAL(isPrime(39), false);
}

STUDENT_TEST("findNthPerfectEuclid test cases") {
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(1)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(2)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(3)));
    EXPECT(isPerfectSmarter(findNthPerfectEuclid(4)));
    EXPECT_EQUAL(findNthPerfectEuclid(1), 6);
    EXPECT_EQUAL(findNthPerfectEuclid(2), 28);
    EXPECT_EQUAL(findNthPerfectEuclid(3), 496);
    EXPECT_EQUAL(findNthPerfectEuclid(4), 8128);
}
