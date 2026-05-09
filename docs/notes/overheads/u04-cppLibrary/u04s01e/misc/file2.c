#include <iostream>
#include <fstream>
#include <vector>

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::vector;

int main()
{
    string filename("input.txt");
    vector<int> numbers;

    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << filename << "'" << endl;
        return EXIT_FAILURE;
    }

    // Wrong: Read twice the last number
    while (!input_file.eof()) {
        int tmp;
        input_file >> tmp;
        numbers.push_back(tmp);
    }

    for (const auto &i : numbers) {
        cout << i << "; ";
    }
    cout << endl;
    input_file.close();

    return EXIT_SUCCESS;
}

