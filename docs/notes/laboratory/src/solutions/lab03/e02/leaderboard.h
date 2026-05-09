//
// Created by michelangelo grosso on 4/4/2023.
//

#ifndef LAB03_LEADERBOARD_H
#define LAB03_LEADERBOARD_H

#include <string>
#include <set>

struct Player {
    std::string name;
    int score;

    Player(const std::string n, int s);

    // Comparison operator for sorting by name and score
    bool operator<(const Player& other) const;

};

class Leaderboard {
public:
    // Add a player to the leaderboard
    void addPlayer(const std::string& name, int score);

    // Print the top N players in the leaderboard
    void printTopPlayers(int n) const;

    // Update the score of a player
    void updateScore(const std::string& name, int newScore);

    // Remove a player from the leaderboard
    void removePlayer(const std::string& name);

private:
    std::set<Player> players;
};


#endif //LAB03_LEADERBOARD_H
