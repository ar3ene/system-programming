//
// Created by michelangelo grosso on 4/11/2023.
//

#ifndef LAB03_SOCIAL_H
#define LAB03_SOCIAL_H

#include <iostream>
#include <map>
#include <set>
#include <string>

typedef struct Vertex {
    std::set<std::string> node;
    bool visited = false;
} vertex_t;

class Social {
public:
    void add_element(const std::string &, const std::string &);
    void display_all() const;
    void display(const std::string &) const;
    bool find_connection(const std::string &, const std::string &);
    void reset_visited();
private:
    std::map<std::string,vertex_t> graph;
};


#endif //LAB03_SOCIAL_H
