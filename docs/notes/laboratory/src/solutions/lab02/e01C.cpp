/*
 * C++ Version
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <memory>

using namespace std;

mutex mtx;

struct Arg {
    int n;
    thread::id tid;
    shared_ptr<struct Arg> parent;
};

void print_backwards(shared_ptr<Arg> i)
{
    if (i->parent != nullptr)
        print_backwards(i->parent);

    cout << i->tid << ' ';
}

void run_thread(shared_ptr<Arg> arg) {
    auto info = make_shared<Arg>();
    info->n = arg->n -1;
    info->tid = this_thread::get_id();
    info->parent = arg;

    if (info->n > 0) {
        thread th1(run_thread, info);
        thread th2(run_thread, info);
        th1.join();
        th2.join();
    }
    else {
        lock_guard<mutex> lock(mtx);
        print_backwards(info->parent);
        cout << endl;
    }
    return;
}

int main(int argc, char *argv[]) {

    int levels;
    auto tidElem = make_shared<Arg>();

    levels = atoi(argv[1]);

    tidElem->tid = this_thread::get_id();
    tidElem->parent = nullptr;
    tidElem->n = levels;

    thread th1(run_thread, tidElem);
    thread th2(run_thread, tidElem);
    th1.join();
    th2.join();

    return 0;
}
