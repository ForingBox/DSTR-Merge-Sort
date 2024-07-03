// Merge Sort
#include <iostream>
#include <fstream>  //for file input and output
#include <sstream>  //for string operations || help parsing in CSV file
#include <vector>   //for using 'vector' container from STL <<Standard Template Library>>
#include <algorithm>
#include <tuple>
#include <chrono>

using namespace std;
using namespace std::chrono;

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

// Function declarations
void extractFile();
void merge(vector<tuple<int,int,int>>& arr, int left, int mid, int right);
void mergeSort(vector<tuple<int,int,int>>& arr, int left, int right);

// Function to extract data from a file, sort it, and write to another file
void extractFile() {
    // Open the input file
    ifstream input_file("mudah-apartment-kl-selangor mmz.csv");
    if (!input_file.is_open()) {
        cerr << "Failed to open input file." << endl;
        return;
    }

    // Open the output file
    ofstream output_file("sorted_size.csv");
    if (!output_file.is_open()) {
        cerr << "Failed to create/open output file." << endl;
        input_file.close();
        return;
    }

    string line;

    // Skip the header line in the input file
    getline(input_file, line);

    vector<tuple<int,int,int>> tuples;  // Vector to store the tuples

    // Process each line in the input file
    while (getline(input_file, line)) {
        stringstream ss(line);
        string cell;
        int column = 0;

        int col7 = 0, col9 = 0, col10 = 0;  // Variables to store the 7th, 9th, and 10th column values
        while (getline(ss, cell, ',')) {
            column++;
            try {
                if (column == 7) {
                    col7 = stoi(cell);  // Convert 7th column to integer
                } else if (column == 9) {
                    col9 = stoi(cell);  // Convert 9th column to integer
                } else if (column == 10) {
                    col10 = stoi(cell);  // Convert 10th column to integer
                }
            } catch (const invalid_argument& e) {
                cerr << "Invalid data at column " << column << ": " << cell << endl;
                col7 = col9 = col10 = 0;  // Set to 0 if there's invalid data
                break;
            }
        }

        // Only add to tuples if all columns were successfully parsed
        if (col7 != 0 && col9 != 0 && col10 != 0) {
            tuples.push_back(make_tuple(col7, col9, col10));
        }
    }

    input_file.close();  // Close the input file after reading all lines

    auto start = high_resolution_clock::now();  // Start the timer

    // Sort the vector of tuples using mergeSort
    mergeSort(tuples, 0, tuples.size() - 1);

    auto stop = high_resolution_clock::now();   // Stop the timer
    auto duration = duration_cast<microseconds>(stop - start);  // Calculate the duration

    // Write sorted data to the output file
    for (const auto& tuple : tuples) {
        output_file << get<0>(tuple) << "," << get<1>(tuple) << "," << get<2>(tuple) << endl;
    }

    output_file.close();  // Close the output file after writing all sorted data

    cout << "Sorting and writing to sorted_size.csv completed" << endl;
    cout << "Time taken: " << duration.count() << " microseconds" << endl;  // Output the duration
}

// Merge function to merge two halves of a vector
void merge(vector<tuple<int,int,int>>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;  // Size of the left subarray
    int n2 = right - mid;  // Size of the right subarray

    // Create temporary subarrays
    vector<tuple<int,int,int>> L(n1);
    vector<tuple<int,int,int>> R(n2);

    // Copy data to temporary subarrays L[] and R[]
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];

    // Merge the temporary subarrays back into arr[left..right]
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        // Comparison based on col7, then col9, then col10
        if (get<0>(L[i]) < get<0>(R[j]) ||
            (get<0>(L[i]) == get<0>(R[j]) && get<1>(L[i]) < get<1>(R[j])) ||
            (get<0>(L[i]) == get<0>(R[j]) && get<1>(L[i]) == get<1>(R[j]) && get<2>(L[i]) < get<2>(R[j]))) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Merge sort function to sort an array
void mergeSort(vector<tuple<int,int,int>>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;  // Find the middle point

        mergeSort(arr, left, mid);  // Sort first half
        mergeSort(arr, mid + 1, right);  // Sort second half

        merge(arr, left, mid, right);  // Merge the sorted halves
    }
}

int main() {
    extractFile();  // Call the extractFile function to process the file

    return 0;
}
