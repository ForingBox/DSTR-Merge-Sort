// Merge Sort
#include <iostream>
#include <fstream>  //for file input and output
#include <sstream>  //for string operations || help parsing in CSV file
#include <vector>   //for using 'vector' container from STL <<Standard Template Library>>
#include <string>   //for using 'string' class

using namespace std;

/* Merge Sort Concept
    1. separate an array into two sub arrays
    2. sort each sub array before combining back
    3. combine while sorting both sub array 
        {the sort is done by comparing the number on the left to right} 
        {if left > right == insert(right) }
        {if left < right ==  insert(left) }*/

/* eg: Given an array --> [ 1, 7, 3, 5, 2, 6, 4]
break into sub arrays   --> a  = [ 1, 7, 3, 5 ]            || b  = [ 2, 6, 4 ] 
keep breaking           --> a1 = [ 1, 7 ] / a2 = [ 3, 5 ]  || b1 = [ 2, 6 ] / b2 = [ 4 ] 
begin sort              --> a1 = [ 1, 7 ] / a2 = [ 3, 5 ]  || b1 = [ 2, 6 ] / b2 = [ 4 ] 
                        --> a1 = [1]                       || b1 = [2]
                            a1 = [1, 3]                    || b1 = [2, 4]
                            a1 = [1, 3, 5]                 || b1 = [2, 4, 6]
                            a1 = [1, 3, 5, 7]              || b1 = [2, 4, 6]
combine both array while sorting it 
a = [1, 3, 5, 7] & b = [2, 4, 6]
sortResult  = [1]                       '1' from a1 -- 1 < 2
            = [1, 2]                    '2' from b1 -- 3 > 2
            = [1, 2, 3]                 '3' from a1 -- 3 < 4
            = [1, 2, 3, 4]              '4' from b1 -- 5 > 4
            = [1, 2, 3, 4, 5]           '5' from a1 -- 5 < 6
            = [1, 2, 3, 4, 5, 6]        '6' from b1 -- 7 > 6
            = [1, 2, 3, 4, 5, 6, 7]     '7' from a1 */



// Function prototypes
void extractFile(vector<int>& sizes);
void mergeSort(vector<int>& arr, int left, int right);
void merge(vector<int>& arr, int left, int mid, int right);

void extractFile(vector<int>& sizes) {
    ifstream file("mudah-apartment-kl-selangor mmz.csv");   //open file
    if (!file.is_open()) {
        cerr << "Failed to open file." << endl;             //print error if file not found or open
        return;
    }

    string line;

    // Skip the header
    getline(file, line);

    // Read and process only the size column <<which is column 10>>
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        int column = 0;
        while (getline(ss, cell, ',')) {
            if (column == 9) {  // Check if it's the 10th column
                try {
                    sizes.push_back(stoi(cell));    //add to 'sizes' vector
                } catch (const exception& e) {
                    cerr << "Conversion error on input: " << cell << " - " << e.what() << endl; //cerr --> character error <<to display error messages>>
                }
                break;
            }
            column++;
        }
    }
    file.close();
}

void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1);
    vector<int> R(n2);

    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

int main() {
    vector<int> sizes;
    extractFile(sizes);

    if (sizes.empty()) {
        cerr << "No sizes to sort" << endl;
        return 1;
    }

    mergeSort(sizes, 0, sizes.size() - 1);

    ofstream outputFile("sorted_sizes_cpp.csv");
    if (!outputFile.is_open()) {
        cerr << "Failed to open output file." << endl;
        return 1;
    }

    for (const int& size : sizes) {
        outputFile << size << endl;
    }
    outputFile.close();

    cout << "Sorting complete. Check sorted_sizes_cpp.csv for results." << endl;

    return 0;
}