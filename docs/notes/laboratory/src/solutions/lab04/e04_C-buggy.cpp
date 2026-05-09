/*
 * Exercise 04 (Version A and B)
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

int is_ready = -1;
mutex mtx;
condition_variable cv;

void thread_func(char *s, vector<unsigned int> *v, int id) {
    ifstream file(s, ios::binary);
    int num;
    file.read(reinterpret_cast<char *>(&num), 4);
    for (int i = 0; i < num; ++i) {
        unsigned int n;
        file.read(reinterpret_cast<char *>(&n), 4);
        v->push_back(n);
    }
    file.close();

    sort(v->begin(), v->end());

    {
        std::lock_guard<std::mutex> lock(mtx);
        is_ready = id;
        cv.notify_one();
    }
    
}

int main(int argc, char *argv[]) {

    auto start = std::chrono::high_resolution_clock::now();

    if (argc <= 1) {
        cout << "No input arguments received.";
        return -1;
    }

    vector<vector<unsigned int>> vet_int(argc - 2);
    vector<thread> vet_thr;

    for (int i = 0; i < argc - 2; ++i) {
        vet_thr.emplace_back(thread_func, argv[i + 1], &vet_int[i], i);
    }

    vector<unsigned int> merged_vector;
    unsigned int num = argc - 2;

    while (num > 0) {
        int i;

        std::unique_lock<std::mutex> lock(mtx);
        while (is_ready == -1) {
            cv.wait(lock);
        }
        i = is_ready;
        is_ready = -1;

        vet_thr[i].join();

        size_t s = merged_vector.size();
        merged_vector.insert(merged_vector.end(), vet_int[i].begin(), vet_int[i].end());
        // inplace_merge(merged_vector.begin(), merged_vector.begin() + s, merged_vector.end());
        sort(merged_vector.begin(), merged_vector.end());
        --num;
        // cout << i << endl;
    }

    ofstream file(argv[argc - 1], ios::out | ios::binary);
    if (!file) {
        cout << "Cannot open file " << argv[1] << "for writing";
        return -1;
    }

    num = merged_vector.size();
    file.write(reinterpret_cast<char *> (&num), 4);

    for (auto &i: merged_vector)
        file.write(reinterpret_cast<char *> (&i), 4);

    file.close();

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Time taken by code: " << duration.count() << " microseconds\n";

    return 0;
}
