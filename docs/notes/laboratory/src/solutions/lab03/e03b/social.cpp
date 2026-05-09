//
// Created by michelangelo grosso on 4/11/2023.
//

#include <fstream>
#include "social.h"

using namespace std;

void Social::add_element(const std::string &a, const std::string &b) {
    if (graph.find(a) == graph.end()) {
        set<string> t{b};
        graph[a].node = t;
    }
    else
        graph[a].node.insert(b);

    if (graph.find(b) == graph.end()) {
        set<string> t{a};
        graph[b].node = t;
    }
    else
        graph[b].node.insert(a);
}

void Social::display_all() const {
    for (auto &i : graph) {
        cout << i.first;
/*        cout << ": ";
        for (auto &j : i.second.node)
            cout << j << " "; */
        cout << endl;
    }
}

void Social::display(const std::string &a) const {
    if (graph.find(a)==graph.end())
        cout << "User " << a << " not found";
    else {
        cout << "Friends of " << a << ": ";
        for (auto &i: graph.at(a).node) {
            cout << i << " ";
        }
        cout << endl;
    }
}

void Social::reset_visited() {
    for (auto &i : graph)
        i.second.visited = false;
}

bool Social::find_connection(const std::string &a, const std::string &b)  {
    bool res = false;
//    cout << "searching for " << a << " and " << b << endl;
    if (a == b)
        res = true;
    else{
        if (!graph.at(a).visited) {
            graph.at(a).visited = true;
            for (auto &i: graph.at(a).node) {
                res = find_connection(i, b);
                if (res) break;
            }
        }
    }
    return res;
}

int main() {
    Social sn;

    ifstream myfile;
    myfile.open("../input1.txt");
    if (!myfile.is_open())
    {
        cout << "Error opening file." << endl;
        return -1;
    }

    string myline;
    while (myfile)
    {
        string a, b;
        myfile >> a >> b;
        if (a.size()!=0)
            sn.add_element(a,b);
    }

    sn.display_all();

    sn.display("s3_n3");

    sn.reset_visited();
    string t1 {"s3_n3"}, t2 {"s5_n5"};
    cout << "Connection between " << t1 << " and " << t2;
    cout << (sn.find_connection(t1, t2) ? " is " : " is not ") << "present." << endl;

    return 0;
}