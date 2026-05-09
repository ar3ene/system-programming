#include <iostream>
#include <fstream>

using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream;

int main()
{
    string filename("input2.txt");
    string s;

    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << filename << "'" << endl;
        return EXIT_FAILURE;
    }

    // Does not read and print newlines
    while (getline(input_file,s)) {
        cout << s << "; ";
    }
    cout << endl;
    
    input_file.close();

    return EXIT_SUCCESS;
}
