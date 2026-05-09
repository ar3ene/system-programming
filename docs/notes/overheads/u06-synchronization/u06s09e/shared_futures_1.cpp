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
    auto task1 = std::async(std::launch::async, [sharedFut]() {
        std::cout << "Task 1 got the result from the shared future: " << sharedFut.get() << "\n";
    });

    auto task2 = std::async(std::launch::async, [sharedFut]() {
        std::cout << "Task 2 got the result from the shared future: " << sharedFut.get() << "\n";
    });

    // The main thread does some other work
    std::cout << "Main thread is doing more work...\n";

    // Wait for the tasks to complete
    task1.get();
    task2.get();

    return 0;
}
