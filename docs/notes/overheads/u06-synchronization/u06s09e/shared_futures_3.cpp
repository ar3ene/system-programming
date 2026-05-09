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

void f1 () {
  // Launch an asynchronous task using std::async
    std::future<int> fut = std::async(std::launch::async, computeSum, 10, 20);

    // Do some other work in the main thread
    std::cout << "Main thread is doing some other work...\n";

    // Get the result from the future (unique future)
    int result1 = fut.get();
    std::cout << "The result from the async task (unique future) is: " << result1 << "\n";

    int result2 = fut.get();
    std::cout << "The result from the async task (unique future) is: " << result2 << "\n";
}

void f2 () {
    // Launch another asynchronous task using std::async and obtain a unique future
    std::shared_future<int> fut2 = std::async(std::launch::async, computeSum, 30, 40);

    std::cout << "Main thread is doing more work...\n";

    // Wait for the tasks to complete
    int result1 = fut2.get();
    std::cout << "The result from the async task (shared future) is: " << result1 << "\n";
    int result2 = fut2.get();
    std::cout << "The result from the async task (shared future) is: " << result2 << "\n";
}

int main() {
  //f1();
  f2();
  return 0;
}
