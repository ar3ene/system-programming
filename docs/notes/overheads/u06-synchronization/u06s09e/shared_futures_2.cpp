#include <iostream>
#include <thread>
#include <future>
#include <chrono>

// Function that will be run asynchronously
int computeSum(int a, int b) {
    // Simulate some work
    std::this_thread::sleep_for(std::chrono::seconds(2));
    return a + b;
}

// Function to use the shared future
void useSharedFuture(std::shared_future<int> sharedFut, const std::string& taskName) {
    std::cout << taskName << " got the result from the shared future: " << sharedFut.get() << "\n";
}

int main() {
    // Launch an asynchronous task using std::async
    std::future<int> fut = std::async(std::launch::async, computeSum, 10, 20);

    // Do some other work in the main thread
    std::cout << "Main thread is doing some other work...\n";

    // Get the result from the future (unique future)
    int result = fut.get();
    std::cout << "The result from the async task (unique future) is: " << result << "\n";

    // Launch another asynchronous task using std::async and obtain a unique future
    std::future<int> fut2 = std::async(std::launch::async, computeSum, 30, 40);

    // Convert the unique future to a shared future
    std::shared_future<int> sharedFut = fut2.share();

    // Launch multiple tasks that use the shared future
    std::thread t1(useSharedFuture, sharedFut, "Task 1");
    std::thread t2(useSharedFuture, sharedFut, "Task 2");

    // The main thread does some other work
    std::cout << "Main thread is doing more work...\n";

    // Wait for the tasks to complete
    t1.join();
    t2.join();

    return 0;
}
