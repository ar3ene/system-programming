//
// Created by michelangelo grosso on 4/4/2023.
//

#include <iostream>
#include "leaderboard.h"


using namespace std;

Player::Player(const std::string& n, int s): name(n), score(s) {}

// Comparison operator <
bool Player::operator<(const Player& other) const {
    // Order by descending score, then by ascending name
    if (score != other.score) {
        return score > other.score;
    }
    return name < other.name;
}

// Add a player to the leaderboard
void Leaderboard::addPlayer(const std::string &name, int score) {
    players.insert({name, score});
}

// Print the top N players in the leaderboard
void Leaderboard::printTopPlayers(int n) const {
    cout << "Leaderboard:" << endl;
    int count = 0;
    for (const auto& player : players) {
        cout << player.name << " - " << player.score << endl;
        count++;
        if (count >= n) {
            break;
        }
    }
}

// Update score
void Leaderboard::updateScore(const std::string &name, int newScore) {
    set<Player>::iterator it;
    for (it = players.begin(); it != players.end(); ++it) {
        if (it->name == name) {
            break;
        }
    }
    if (it == players.end())
        cout << name << " not found" << endl;
    else
    {
        players.erase(it);
        players.insert({name, newScore});
    }

}

// Remove a player from the leaderboard
void Leaderboard::removePlayer(const std::string& name) {
    set<Player>::iterator it;
    for (it = players.begin(); it != players.end(); ++it) {
        if (it->name == name) {
            break;
        }
    }
    if (it == players.end())
        cout << name << " not found" << endl;
    else
        players.erase(it);
}

int main() {
    Leaderboard leaderboard;

    // Add some players with scores
    leaderboard.addPlayer("Alice", 100);
    leaderboard.addPlayer("Bob", 50);
    leaderboard.addPlayer("Charlie", 75);
    leaderboard.addPlayer("Dave", 75);
    leaderboard.addPlayer("Eve", 25);

    // Print leaderboard
    leaderboard.printTopPlayers(5);

    // Update a player's score
    leaderboard.updateScore("Charlie", 60);

    // Remove a player from the leaderboard
    leaderboard.removePlayer("Alice");

    // Print updated leaderboard
    leaderboard.printTopPlayers(3);

    return 0;
}

