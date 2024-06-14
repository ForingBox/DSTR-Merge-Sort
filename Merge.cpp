#include <iostream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;



int extractFile(){
    ifstream file("mudah-apartment-kl-selangor mmz.csv");
        if (!file.is_open()) {
            cerr << "Failed to open file." << endl;
            return 1;
        }

    string line;
    vector<int> sizes;

    // Skip the header
    getline(file, line);

    // Read and process only the J column (assuming it's the 10th column, 0-indexed)
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        int column = 0;  // Start counting columns from 0
        while (getline(ss, cell, ',')) {
            if (column == 9) {  // Check if it's the 10th column
                try {
                    sizes.push_back(stoi(cell));
                } catch (const std::exception& e) {
                    cerr << "Conversion error on input: " << cell << " - " << e.what() << endl;
                }
                break;  // Stop reading more cells once we get the 10th column
            }
            column++;
        }
    }
    file.close();

    // Optionally sort the sizes
    // sort(sizes.begin(), sizes.end());

    // Output the data to a new file
    ofstream output("sorted_sizes_cpp.csv");
    if (!output.is_open()) {
        cerr << "Failed to open output file." << endl;
        return 1;
    }

    output << "size\n";  // Write header
    for (int size : sizes) {
        output << size << endl;
    }
    output.close();

    cout << "Sizes extracted and saved to sorted_sizes_cpp.csv" << endl;
    return 1;
}

int main(){
    extractFile();
}