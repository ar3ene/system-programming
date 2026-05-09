#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <list>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::list;
using std::ifstream;
using std::unordered_set;
using std::unordered_map;

class node_c {
  public:
    int visited;
    string pred;
    unordered_set<string> adjacency;

    node_c() {
        visited = 0;
        pred = "";
        adjacency = {};
    }
};

void graph_read (char *argv, unordered_map<string,node_c> &graph);
void graph_display (unordered_map<string,node_c> &graph);
void graph_list_node (unordered_map<string,node_c> &graph);
void graph_adjacency (unordered_map<string,node_c> &graph, string name);
int graph_dfs (unordered_map<string,node_c> &graph, string name1, string name2);
int graph_bfs (unordered_map<string,node_c> &graph, string name1, string name2);

int main(int argc, char *argv[]) {
    int ret;
    ifstream is;
    string name, name1, name2;
    node_c node, node1, node2;
    unordered_map<string,node_c> graph;

    graph_read (argv[1], graph);
    graph_display (graph);
    graph_list_node (graph);

    do {
        cout << "Search node and display adjacency list (\"end\" to tarminate): ";
        cin >> name;
        if (graph.count(name)==0) {
            cout << "No node." << endl;
        } else {
            graph_adjacency(graph, name);
        }
    } while (name!="end");

    do {
        cout << "Search friendship (\"end end\" to terminate): ";
        cin >> name1 >> name2;
        if (graph.count(name1)==0 || graph.count(name2)==0) {
            cout << "No node (" << name1 << " or " << name2 << ") in the network." << endl;
        } else {
            // Depth-First Search
            cout << "Graph DFS (Depth-First Visit): ";
            ret = graph_dfs(graph, name1, name2);
            if (ret == 1) {
                cout << name1 << endl;
            }
            for (auto &eg: graph) {
                eg.second.visited = 0;
            }

            // Breadth-First Search
            cout << "Graph BFS (Breadth-First Visit): ";
            ret = graph_bfs(graph, name1, name2);
            if (ret == 1) {
                do {
                    cout << name2 << " ";
                    name2 = graph[name2].pred;
                } while (graph[name2].pred!="");
                cout << name1 << endl;
            }
            for (auto &eg: graph) {
                eg.second.visited = 0;
                eg.second.pred = "";
            }

        }
    } while (name1!="end" && name2!="end");

    return 0;
}

void graph_read (char *argv, unordered_map<string,node_c> &graph) {
    string filename(argv);
    ifstream is;
    string name, name1, name2;
    node_c node, node1, node2;

    is.open(filename);
    if (!is.is_open()) {
        cout << "Error opening file " << argv[1] << endl;
    }
    cout << "Reading graph:" << endl;
    while (is >> name1 >> name2) {
        cout << name1 << " " << name2 << endl;
        if (graph.count(name1)==0) {
            node.visited = 0;
            node.adjacency.insert(name2);
            graph[name1] = node;
        } else {
            graph[name1].adjacency.insert(name2);
        }
        if (graph.count(name2)==0) {
            node.visited = 0;
            node.adjacency.insert(name1);
            graph[name2] = node;
        } else {
            graph[name2].adjacency.insert(name1);
        }
    }
    is.close();
    return;
}

void graph_display (unordered_map<string,node_c> &graph) {
    cout << "Printing graph:" << endl;
    for (auto &eg: graph) {
        cout << eg.first << ":" << eg.second.visited << ":";
        for (auto &ea: eg.second.adjacency) {
            cout << ea << " ";
        }
        cout << endl;
    }
    return;
}

// Print sorted adjacency list
// May also use (sorted) map instead of unordered map
void graph_list_node (unordered_map<string,node_c> &graph) {
    vector<string> v;

    cout << "Printing sorted node list:" << endl;
    for (auto &eg: graph) {
        v.push_back(eg.first);
    }
    std::sort(v.begin(), v.end());
    for (auto &ev: v) {
        cout << ev << " ";
    }
    cout << endl;

    return;
}

// Print sorted adjacency list
// May also use (sorted) set instead of unsorted_set
void graph_adjacency (unordered_map<string,node_c> &graph, string name) {
    vector<string> v;
    node_c node;

    node = graph[name];
    cout << "Sorted adjacency list: ";
    for (auto &ea: node.adjacency) {
        v.push_back(ea);
    }
    std::sort(v.begin(), v.end());
    for (auto ev: v) {
        cout << ev << " ";
    }
    cout << endl;
    return;
}

int graph_dfs (unordered_map<string,node_c> &graph, string name1, string name2) {
    int ret;
    static int flag = 0;
    node_c node;

    //graph_display(graph);
    //cout << "Visiting " << name1 << " " << name2 << endl;

    if (name1==name2) {
        return 1;
    }

    if (graph[name1].visited == 1) {
        return 0;
    }
    graph[name1].visited = 1;
    node = graph[name1];
    for (auto &ea: node.adjacency) {
        ret = graph_dfs (graph, ea, name2);
        if (ret==1) {
            cout << ea << " ";
            return 1;
        }
    }

    return 0;
}

int graph_bfs (unordered_map<string,node_c> &graph, string name1, string name2) {
    int ret;
    string name;
    list<string> queue;
    static int flag = 0;
    node_c node;

    queue.push_front(name1);
    graph[name1].visited = 1;
    while (!queue.empty()) {
        name = queue.back();
        // cout << "From node: " << name << "to nodes ";
        queue.pop_back();
        node = graph[name];
        for (auto &ea: node.adjacency) {
            if (graph[ea].visited == 0) {
                // cout << " - " << ea;
                graph[ea].visited = 1;
                graph[ea].pred = name;
                if (ea == name2) {
                    return 1;
                }
                queue.push_front (ea);
            }
        }
        //cout << endl;
    }

    return 0;
}