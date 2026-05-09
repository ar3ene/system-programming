/*
Exercise 05 (A and B)
Multiple matrix multiplications
-------------------------------

Implement a *sequential* program in C that:
- Takes a single argument n from the command line
- Generates two arrays (v1 and v2) of size n and a matrix (mat) of
        dimension (n x n), assigning to all elements random values.
All random values must be in the range [-0.5, 0.5]
- Evaluates the following product
res = v1^T * mat * v2
        (where v1^T is the transpose of v1).
The computation must be performed in two steps, as the program must first
compute
        v = v1^T * mat
            and then evaluate
        res = v * v2
              - Prints the result res.

For example, let us suppose that n=5, and v1, v2 and mat are the
        following ones:

v1^T = [ -0.0613 -0.1184  0.2655  0.2952 -0.3131 ]

mat  = [ -0.3424 -0.3581  0.1557  0.2577  0.2060
0.4706 -0.0782 -0.4643  0.2431 -0.4682
0.4572  0.4157  0.3491  0.1078 -0.2231
-0.0146  0.2922  0.4340 -0.1555 -0.4029
0.3003  0.4595  0.1787 -0.3288 -0.4656 ]

v2^T = [ -0.3235  0.1948 -0.1829  0.4502 -0.4656 ]

Then, the result of the computation is:

res = v1^T * mat * v2 = (v1^T * mat) * v2 = -0.004680

After the sequential program correctly computes the final result,
transform it into two *concurrent* programs using Pthreads.

Organize the version A of the concurrent program as follows:

- The main thread creates the arrays, the matrix, and it runs n threads.
Then, it waits for the termination of all n threads.
- Each thread performs the product of the array v1^T with i-th row
of mat, which produces the i-th element of vector v.
- When all threads have terminated their task, the main thread compute
the final result as
res = v * v2

Organize the version B of the concurrent program as version A, but
        once v has been computed, the final result (i.e., res = v * v2) is computed
by one of the created threads (not by the main threads).
Force the program to use the last thread which terminates the first
part of the task to compute the second part.

*/

#include <iostream>
#include <random>
#include <sstream>
#include <chrono>

using namespace std;

int main(int argc, char *argv[]) {

    std::random_device rd;           // Inizializza un generatore di numeri casuali
    std::mt19937 mt(rd());          // Utilizza il generatore Mersenne Twister
    std::uniform_real_distribution<double> distr(-0.5, 0.5); // nota: estremo superiore non incluso

    int num;
    stringstream(argv[1]) >> num;

    double *v1 = new double [num];

    for (int i=0; i<num; i++)
    {
        v1[i] = distr(mt);
    }

    double **mat = new double *[num];
    for (int i=0; i<num; i++) {
        mat[i] = new double [num];
        for (int j=0; j<num; j++)
            mat[i][j] = distr(mt);
    }

    double *v2 = new double[num];

    for (int i=0; i<num; i++)
    {
        v2[i] = distr(mt);
    }

   /* double v1[5] = { -0.0613, -0.1184,  0.2655,  0.2952, -0.3131 };

    double **mat = new double *[num];
    mat[0] = new double [5] {-0.3424, -0.3581,  0.1557,  0.2577,  0.2060};
    mat[1] = new double [5] {0.4706, -0.0782, -0.4643,  0.2431, -0.4682};
    mat[2] = new double [5] {0.4572,  0.4157,  0.3491,  0.1078, -0.2231};
    mat[3] = new double [5] {-0.0146,  0.2922,  0.4340, -0.1555, -0.4029};
    mat[4] = new double [5] {0.3003,  0.4595,  0.1787, -0.3288, -0.4656};

    double v2[5] = {-0.3235,  0.1948, -0.1829,  0.4502, -0.4656}; */

    double *vtemp = new double [num]();

    auto start = std::chrono::high_resolution_clock::now();

    for (int i=0; i<num; ++i) {
        for (int j=0; j<num; ++j) {
            vtemp[i]+=v1[j]*mat[j][i];
        }
    }

    double res(0);
    for (int i=0; i<num; i++) {
        res += vtemp[i] * v2[i];
    }

    cout << "Result: " << res << endl;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Time taken by code: " << duration.count() << " microseconds\n";

    return 0;
}