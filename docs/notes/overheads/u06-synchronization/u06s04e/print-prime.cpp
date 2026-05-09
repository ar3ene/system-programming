#include <iostream>
#include <thread>
#include <vector>

bool check_prime(int n) {

    if (n <= 1){
        std::cout << n << " is NOT prime" << std::endl;
        return false;
    }

    // Check from 2 to n-1
    for (int j=2; j<n; j++) {
        if (n % j == 0) {
            std::cout << n << " is NOT prime" << std::endl;
            return false;
        }
    }

    std::cout << n << " is prime" << std::endl;
    return true;
}


int main(int argc, char *argv[] ) {

    if(argc != 3){
        std::cerr <<"Launch program with two arguments: lower and upper bound of the range" << std::endl;
        return -1;
    }

    //for simplicity, no check on data correctness
    const unsigned int lb = atoi(argv[1]);
    const unsigned int ub = atoi(argv[2]);

    const unsigned int max = std::thread::hardware_concurrency();
    std::cout << "Prime numbers between  " <<
        lb << " and " << ub <<  " with max threads = " << max <<  std::endl;

    unsigned int const d = ub-lb+1; //numbers to check if prime

    unsigned int const n_threads = max>d?d:max;
    unsigned int const n_loops = d<=n_threads?(d/n_threads):(d/n_threads)+1;


    unsigned int lb_loop = lb;
    unsigned int ub_loop = n_threads;

    std::vector<std::thread> threadPool;

    for(int l=0; l<n_loops; ++l) {


        for (int i = lb_loop; i < ub_loop; ++i) {

                threadPool.emplace_back([i] { check_prime(i); });
        }
        for (auto& t : threadPool) {
            t.join();
        }
        threadPool.clear(); //Removes and destroys all elements from the vector, which then is size 0
        lb_loop = ub_loop;
        ub_loop = ub_loop+n_threads>ub?ub+1:ub_loop+n_threads+1;
    }
    return 0;
}
