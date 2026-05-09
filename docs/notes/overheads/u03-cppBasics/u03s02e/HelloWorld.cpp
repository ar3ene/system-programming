#include <iostream>

using namespace std;

int main(int argc, char ** argv) {
    int vi;
    float vf;
    if (argc != 1) {
        cerr << "Parameter Error!" << endl;
        return -1;
    }
    cout << "Insert variables: " << endl;
    cin >> vi >> vf;
    cout << "Values: " << vi << " " << vf << endl;

    /* int& a  = vi;
    a++;
    cout << "New value of vi: " << vi << endl;
    */

    return 0;
}

