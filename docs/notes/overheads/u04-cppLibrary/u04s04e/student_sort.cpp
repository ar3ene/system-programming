/*
 * Sorting students with lambda functions and the sort algorithm
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

struct Student {
    int id;
    std::string name;
    double grade;
};

// Function to sort student records based on ID in ascending order
void sortByID(std::vector<Student>& students) {
    std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.id < b.id;
    });
}

// Function to sort student records based on name in alphabetical order
void sortByName(std::vector<Student>& students) {
    std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.name < b.name;
    });
}

// Function to sort student records based on grade in descending order
void sortByGrade(std::vector<Student>& students) {
    std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
        return a.grade > b.grade;
    });
}

// Function to display student records
void displayRecords(const std::vector<Student>& students) {
    for (const auto& student : students) {
        std::cout << "ID: " << student.id << ", Name: " << student.name << ", Grade: " << student.grade << std::endl;
    }
}

int main() {
    std::cout << "Welcome to the Student Record Management System!\n\n";

    std::vector<Student> students;
    int id;
    std::string name;
    double grade;

    std::cout << "Student records (ID=0 to stop, Name, Grade):\n";
    while (true) {
        std::cout << "> ";
        std::cin >> id >> name >> grade;
	if (id==0)
	  break;
        students.push_back({id, name, grade});
    }

    int choice;
    std::cout << "\nChoose sorting criteria:\n";
    std::cout << "1. Sort by ID\n";
    std::cout << "2. Sort by Name\n";
    std::cout << "3. Sort by Grade\n";
    std::cout << "\nEnter your choice: ";
    std::cin >> choice;

    switch (choice) {
        case 1:
            sortByID(students);
            std::cout << "\nSorted Student Records by ID (ascending order):\n";
            break;
        case 2:
            sortByName(students);
            std::cout << "\nSorted Student Records by Name (alphabetical order):\n";
            break;
        case 3:
            sortByGrade(students);
            std::cout << "\nSorted Student Records by Grade (descending order):\n";
            break;
        default:
            std::cout << "Invalid choice.\n";
            return 1;
    }

    displayRecords(students);

    return 0;
}

