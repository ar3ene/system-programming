/*
Exercise 02
Exam 5 July 2021 – Thread communication with future and promises
--------------
Write a small C++ example with three threads:
- thread take_value takes a number from console
- thread check_if_prime checks whether the number is prime
- thread give_answer prints the answer to standard output
Thread communication should be made using promises and futures.

*/

#include <iostream>
#include <thread>
#include <vector>
#include <future>



void take_value(std::promise<int>& p_user_input)
{
    int user_input_value;
    {
        std::cout << "Please insert a number" << std::endl;
        std::cin >> user_input_value;
        p_user_input.set_value( user_input_value );

    }


}
void check_if_prime(std::future<int>& f_value, std::promise<bool>& p_result)
{

    int n = f_value.get();
    bool prime=true;

    if (n <= 1){
        prime=false;
    }

    // Check from 2 to n-1
    for (int j=2; j<n/2; j++) {
        if (n % j == 0) {
            prime=false;
            break;
        }
    }

    p_result.set_value(prime);
}

void give_answer(std::future<bool>& computation)
{
    bool answer = computation.get();
    std::string s0 (" ");

    if(!answer)
        s0=" NOT";

    std::cout << "Number is" << s0 << " prime";
}

int main(){


    std::promise<int> p_user_input;
    std::thread t1(take_value, std::ref(p_user_input));

    std::future<int> f_user_input = p_user_input.get_future();
    std::promise<bool> p_result;
    std::thread t2(check_if_prime, std::ref(f_user_input), std::ref(p_result));

    std::future<bool> f_result = p_result.get_future();
    std::thread t3(give_answer, std::ref(f_result));

    t1.join();
    t2.join();
    t3.join();

    return 0;
}

