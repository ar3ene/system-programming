/*
Exercise 01
C++ async
--------------
Implement the solution to Exercise 05 in Lab 04 (version A) using asyncs.

*/

#include <iostream>
#include <random>
#include <sstream>
#include <thread>
#include <future>

using namespace std;

double async_func(double *a, double **b, int k, int n) {
    double t = 0;
    for (int i=0; i<n; i++)
        t+=a[i]*b[i][k];
    return t;
}

int main(int argc, char *argv[]) {

    std::random_device rd;           // random number generator initialization
    std::mt19937 mt(rd());           // Mersenne Twister
    std::uniform_real_distribution<double> distr(-0.5, 0.5); // note: upper bound not included

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

    //double *vtemp = new double [num]();

    auto start = std::chrono::high_resolution_clock::now();

    //vector<thread> vet_thr;

    //for (int i=0; i<num; ++i) {
    //    vet_thr.emplace_back(thread(thread_func, v1, mat, vtemp, i, num));
    //}

    vector<future<double>> vtemp;
    for (int i=0; i<num; ++i) {
        vtemp.emplace_back(async(async_func, v1, mat, i, num));
    }

    //for (auto &i : vet_thr)
    //    i.join();

    double res(0);
    for (int i=0; i<num; i++) {
        res += vtemp[i].get() * v2[i];
    }

    cout << "Result: " << res << endl;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "Time taken by code: " << duration.count() << " microseconds\n";

    return 0;
}