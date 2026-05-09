/*
 * Exercise: Word Frequency Counter
 */

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#include <cctype>

int main() {
    std::string paragraph;
    std::string word;
    std::set<std::string> words;
    std::map<std::string, int> wordFrequencyMap;

    // Read a paragraph
    std::cout << "Enter a paragraph of text: ";    
    std::getline(std::cin, paragraph);

    // Tokenize the paragraph and count word frequencies
    std::stringstream ss(paragraph);
    while (ss >> word) {
        // Remove punctuation and convert to lowercase
        std::string clean_word;
        for (char c : word) {
            if (std::isalpha(c)) {
                clean_word += std::tolower(c);
            }
        }
        words.insert(clean_word);
        wordFrequencyMap[clean_word]++;	  
    }

    // Display unique words and their frequencies
    std::cout << "\nUnique words and their frequencies:\n";
    for (const auto& pair : wordFrequencyMap) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    // Display total number of unique words
    std::cout << "\nTotal number of unique words: " << words.size() << std::endl;

    // Search for a word and display its frequency
    std::cout << "\nEnter a word to search its frequency: ";
    std::cin >> word;
    std::cout << "The word \"" << word << "\" appears " << wordFrequencyMap[word] << " times in the paragraph.\n";

    // Display unique words sorted alphabetically
    std::cout << "\nUnique words sorted alphabetically:\n";
    for (const std::string &word : words) {
        std::cout << word << std::endl;
    }

    return 0;
}

