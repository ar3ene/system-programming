/*
 * C++ Version
 */

#define R_PAR 3
#define X_PAR 4
#define C_PAR 5

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;

void run_thread(int **A, int **B, int **C,  int r, int c, int x) {
    int i, res=0;

    for (i=0; i<x; i++)
        res += A[r][i] * B [i][c];

    C[r][c] = res;
    return;
}

void mat_mul(int **A, int **B, int r, int x, int c, int **C) {
    int i, j, k;

    vector<thread> th;

    for (i=0, k=0; i<r; i++)
        for (j=0; j<c; j++, k++) {

            C[i][j] = 0;
            thread temp_th(run_thread, A, B, C, i, j, x);
            th.emplace_back(move(temp_th));
    }
    for (i=0; i<r*c; i++)
        th[i].join();
}



int main(void) {
    int i, j;
    int **A, **B, **C;

    A = new int* [R_PAR];

    for (i=0; i<R_PAR; ++i)
        A[i] = new int[C_PAR];

    int mat_A[R_PAR][X_PAR] = {{1, 2,  3,  4},
                               {5, 6,  7,  8},
                               {9, 10, 11, 12}};
    for (i=0; i< R_PAR; i++)
        for (j=0; j<X_PAR; j++)
            A[i][j]=mat_A[i][j];

    B = new int* [X_PAR];
    for (i=0; i<X_PAR; i++)
        B[i] = new int[C_PAR];

    int mat_B[X_PAR][C_PAR] = {{13, 14, 15, 16, 17},
                               {18, 19, 20, 21, 22},
                               {23, 24, 25, 26, 27},
                               {28, 29, 30, 31, 32}};
    for (i=0; i< X_PAR; i++)
        for (j=0; j<C_PAR; j++)
            B[i][j]=mat_B[i][j];

    C = new int* [R_PAR];
    for (i=0; i<R_PAR; i++)
        C[i] = new int[C_PAR];

    /* expected result:
    C = {{230, 240, 250, 260, 270},
     {558, 584, 610, 636, 662},
     {886, 928, 970, 1012, 1054}}
     */

    mat_mul(A, B, R_PAR, X_PAR, C_PAR, C);

    for (i=0; i<R_PAR; i++){
        for (j=0; j<C_PAR; j++)
            cout << C[i][j] << '\t';
        cout << endl;
    }

    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}