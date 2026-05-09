#include <string>
#include <vector>
#include <iostream>

using namespace std;

int main (int argc, char ** argv) {
    vector<string> v;

    cout << "Vector example (C++):" << endl;    
    string s2 = "one";
    v.push_back(s2);
    s2 = "two";
    v.push_back(s2);
    s2 = "three";
    v.push_back(s2);
    for (long unsigned int i = 0; i < v.size(); ++i) {
      cout << "  " << v[i] << '\n';
    }

    return (0);
}
