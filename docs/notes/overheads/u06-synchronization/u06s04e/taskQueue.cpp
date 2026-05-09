//
// Created by Antonio Vetrò on 09/05/22.
//
#include "condition_variable"
#include "iostream"
#include "queue"
#include "thread"


using namespace std;

mutex m;
condition_variable cv;
queue<int> taskQueue;
bool lastreader;
int const NREADERS = 9;
int readers_count;

void pushWork(int task) {
    if(task%2) this_thread::sleep_for(chrono::milliseconds(200));

    unique_lock l{m};

    taskQueue.push(task);
    cout << "Pushed " << task << endl;

    readers_count++;
    if(readers_count == NREADERS)
        lastreader=true;

    l.unlock();
    cv.notify_one();
}

void workerThread() {

    unique_lock l{m};
    while(true) {

        if (!taskQueue.empty())
        {
            int task =	taskQueue.front();
            taskQueue.pop();
            cout << "==== Worked " << task << "====" <<  endl;
            l.unlock();

            l.lock();
            if(taskQueue.size()==0 & lastreader) {
                cout << endl <<  "**** No elements in the queue, no further readers, leaving program ****" << endl;
                return;
            }
        }
        cout << "==== Ready to wait ==== queue size: " << taskQueue.size() <<  endl;
        if(!lastreader)
         cv.wait(l);
    }
}

int main(){

    thread worker(workerThread);

    vector<thread> threadPool;
    lastreader=false;
    readers_count=0;

    for (int i = 1; i <= 9; ++i) {

        threadPool.emplace_back([=] { pushWork(i); });

    }
    for (auto& t : threadPool) {
        t.join();
    }
    worker.join();

}