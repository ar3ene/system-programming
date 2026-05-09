//
// Created by Antonio Vetro' on 09/04/22.
//

#include "iostream"

using namespace std;

    int sum(int a, int b){ return a+b;};
    double sum(double a, double b){ return a+b;};;

    int f() { return 42; }


int main() {
        int a = 42, y;
        double b = 3.14, x, z;

         /* x = sum(a, b); */
         y = sum(a, (static_cast<int>(b)));
         z = sum((static_cast<double>(a)), b);
         cout << "y = " << y << endl << "z = " << z << endl;


        int i = 42;
        void* vp = &i;
        int* ip = static_cast<int*>(vp);
        cout << "ip = " << ip << endl << "vp = " << vp << endl;

        uintptr_t v1 = reinterpret_cast<uintptr_t>(&i);
        cout << "The value of &i is 0x" << hex << v1 << '\n';

        void(*fp1)() = reinterpret_cast<void(*)()>(f);
        cout << "fp1: " <<  fp1 << endl;

        int(*fp2)() = reinterpret_cast<int(*)()>(fp1);
        cout << "fp2: " << dec << fp2() << '\n';

        i = 7;
        char* p2 = reinterpret_cast<char*>(&i);
        if (p2[0] == '\x7')
            cout << "This system is little-endian\n";
        else
            cout << "This system is big-endian\n";

        reinterpret_cast<unsigned int&>(i) = 42;
        cout << i << '\n';


};
