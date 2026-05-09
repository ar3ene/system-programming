/*
Exercise 02
Prefix sum
-----------

Given an array v of 2^n elements v[i], i.e.,

v = { v[0], v[1], ... , v[2^n − 1] }

its k-th prefix sum (the cumulative sum vector) is the sum of its first k+1
elements, i.e.,
v[0] + v[1] + ... + v[k]

Write a C program using Pthreads that:
* Receives an integer value n as an argument of the command line
* Randomly generates an array v of 2^n integer elements in the range [1-9]
* Computes the prefix sum of v as follows.

For an array v of 2^n elements, it is possible to compute all values of
        the prefix sum in n steps, by means of 2^n−1 concurrent threads, each
associated with an array element.

For example for n=3 and 2^3 = 8 elements, each step consists of each
        thread taking two elements, and computing a result:
* In the first step, each thread computes the sums of adjacent elements
(i.e., gap = 1=2^0).
* In the second step, each thread computes the sums of the elements
        that are 2 elements away (i.e., gap = 2 = 2^1).
* In the third step, each thread computes the sums of the elements
        that are 4 elements away (i.e., gap = 4 = 2^2).
After 3 steps, all sum prefix values are computed.

In general, if we have 2^n elements in v, step i computes the sums of
        elements that are 2^(i-1) elements away.
After n steps, all cumulative sum values are computed.

For example, let us suppose the program is run as:

> cumulative_sum  3

It should compute (and eventually display) the following:

Initial array v:  2  4  6  1  3  5  8  7
v after step 1 :  2  6 10  7  4  8 13 15
Computed as: v[0]=v[0], v[1]=v[0]+v[1],
v[2]=v[1]+v[2], etc., v[n-1]=v[n-2]+v[n-1]
(sum of elements at distance 2^0=1)
v after step 2 :  2  6 12 13 14 15 17 23
Computed as: v[0]=v[0], v[1]=v[1], v[2]=v[0]+v[2],
v[3]=v[1]+v[3], etc., v[n-1]=v[n-3]+v[n-1]
(sum of elements at distance 2^1=2)
v after step 3 :  2  6 12 13 16 21 29 36
Computed as: v[0]=v[0], v[1]=v[1], v[2]=v[2], v[3]=v[4],
v[4]=v[0]+v[4], v[5]=v[1]+v[5], etc.,
v[n-1]=v[n-5]+v[n-1]
(sum of elements at distance 2^2=4)
There is no step 4 as in step 4 the procedure should sum elements at a
        distance 2^3=8 (which is the size of the array).

Notice that in the final array:
- v[7]=36 is the sum of elements v[0]+v[1]+...+v[7]
- v[6]=29 is the sum of elements v[0]+v[1]+...+v[6]
- v[5]=21 is the sum of elements v[0]+v[1]+...+v[5]
- etc.

Moreover, notice that:
1. The main thread allocates, fills, and prints the initial content of
the array v
2. All threads are initially created by the main thread, they have all
        the same code and they must synchronize their activity according to the
procedure previously described, exploiting the maximum possible
concurrency.
3. The array v must be updated in place, i.e., you cannot allocate
        auxiliary arrays.
4. Each thread must loop for no more than n times (the number of
steps) before exiting.
5. Some threads terminate before the n-th step.
In particular thread i terminates after step i, with i in the
        range [1–n].
6. The main thread prints immediately the value in v[0], then it waits
        the termination of each thread i, in order of creation, and
        immediately prints the value stored in v[i].

Suggestions
-----------
* Perform synchronization using barriers
* First design and write the pseudo-code of a solution excluding rule 5.
* Then, update your solution including rule 5.
Please notice that:
- If you refer to v[i-gap], index i-gap must be non-negative
- You must properly manage a variable that stores the number of
        active threads, which must be decremented according to rule 5.

*/

#include <iostream>
#include <sstream>
#include <random>
#include <barrier>

using namespace std;

void thread_func(vector<int> &v, int n, int dim, barrier<> &bar){
    int sum;
    for (int i=0; i<dim ; ++i)
    {
        int p = n- (1 << i);
        if (p<0) break;
        sum = v[n]+v[p];
        bar.arrive_and_wait();
        v[n] = sum;
        bar.arrive_and_wait();
    }
    bar.arrive_and_drop();
}

int main(int argc, char *argv[]) {

    std::random_device rd;           // random number generator initialization
    std::mt19937 mt(rd());          //  Mersenne Twister
    std::uniform_int_distribution<unsigned int> dist(1, 9);

    int num, pnum;
    stringstream(argv[1]) >> num;
    pnum = 1 << num;

    vector<int> vet;

    for (int i=0; i<pnum; i++)
        vet.emplace_back(dist(mt));

    cout << "Original vector: ";
    for (auto n : vet)
        cout << n << " ";
    cout << endl;


    vector<thread> vt(pnum-1);

    barrier bar(pnum-1);

    for (int i=0; i<pnum-1; ++i) {
        vt[i] = thread(thread_func, ref(vet), i+1, num, ref(bar));
    }

    cout << "Modified vector: ";
    int i=0;
    for (auto &t : vt) {
        cout << vet[i++] << " ";
        t.join();
    }

    cout << vet[i] << endl;

    return 0;
}