//
// Created by Antonio Vetrò on 09/05/22.
//

#include "thread"
#include "iostream"
#include "vector"
#include "atomic"
using namespace std;

std::mutex printMutex;
std::recursive_mutex m;


void safe_print(int i){
   // printMutex.lock();
    cout << i << endl ;
    // printMutex.unlock();

}

// RECURSIVE MUTEX
void foo() {
    m.lock();
    std::cout << "foo\n";
    m.unlock();
}

void bar() {
    m.lock();
    std::cout << "bar\n";
    foo(); // This will not deadlock
    m.unlock();
}

// CONSISTENT ORDER

mutex m1, m2, m3;
void threadA() {
    this_thread::sleep_for(chrono::seconds(3));
    std::unique_lock l1{m1}, l2{m2}, l3{m3};
   //scoped_lock sl1{m1, m2,m3};

    cout << "A" << endl;
}

void threadB() {
    this_thread::sleep_for(chrono::seconds(1));
    std::unique_lock l3{m3}, l2{m2}, l1{m1};
    // DANGER: order not consistent with threadA()
    //std::scoped_lock sl1{m3, m2, m1};

    cout << "B" << endl;
}

// ATOMIC OPERATIONS

std::atomic<int> ia = 0;

void workerThread() {
    ia.fetch_add(1); // (A)
    ia.fetch_sub(1); // (B)
}

void readerThread() {
    int iLocal = ia.load();
    if(iLocal == 0 || iLocal == 1)
        std::cout << "Memory Consistent!" << endl;
    else
        std::cout << "Memory NOT Consistent!" << endl;
}



int main(){


   vector<thread> threadPool;
    for (int i = 1; i <= 9; ++i) {
        threadPool.emplace_back([i] { safe_print(i); });
    }

    for (auto& t : threadPool) {
    t.join();
    }

    thread t1([] { std::cout << "Hi\n"; });
    thread t2 = std::move(t1);

    //t1.join();// it will be not possible to join t1, since it has been moved to t2
    t2.join();

    //RECURSIVE MUTEX
    cout << "*** RECURSIVE MUTEX *** " << endl;

    thread t3(bar);
    thread t4(foo);
    thread t5(bar);

    //you could try not joining threads and check what happens
    t3.join();
    t4.join();
    t5.join();


    //SHARED MUTEX
    cout << "*** SHARED MUTEX *** " << endl;

    int value = 0;
    std::timed_mutex ms; // shared mutex not available in the mac used for writing this code

    std::vector<std::thread> tPool;

    // Add readers
    for (int i = 0; i < 5; ++i){
        tPool.emplace_back([&] {
            ms.lock();
            safe_print(value);
            ms.unlock();
        });
    }

    // Add writers
    for(int i = 0; i < 5; ++i){
        tPool.emplace_back([&] {
            ms.lock();
            ++value;
            safe_print(value);
            ms.unlock();
        });
    }

    for (auto& t : tPool) {
        t.join();
    }

    // CONSISTENT ORDER
    cout << "*** CONSISTENT ORDERS *** " << endl;

    thread TA(threadA);
    thread TB(threadB);

    TA.join();
    TB.join();

    //ATOMIC OPERATIONS
    cout << "*** ATOMIC OPERATIONS *** " << endl;
    thread R(readerThread);
    thread W(workerThread);
    //thread R2(readerThread);
   // thread R3(readerThread);

    R.join();
    W.join();
    //R2.join();
    //R3.join();

}