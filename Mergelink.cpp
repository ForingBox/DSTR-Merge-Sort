#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Define the node structure for the linked list
struct Node {
    tuple<int, int, int> data;
    Node* next;
};

// Function to create a new node
Node* createNode(int col7, int col9, int col10) {
    Node* newNode = new Node();
    newNode->data = make_tuple(col7, col9, col10);
    newNode->next = nullptr;
    return newNode;
}

// Function to insert a node at the end of the linked list
void appendNode(Node*& head, int col7, int col9, int col10) {
    Node* newNode = createNode(col7, col9, col10);
    if (!head) {
        head = newNode;
    } else {
        Node* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Function declarations for merge sort
Node* sortedMerge(Node* a, Node* b);
void splitList(Node* source, Node** frontRef, Node** backRef);
void mergeSort(Node** headRef);

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

    Node* head = nullptr;

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

        // Only add to linked list if all columns were successfully parsed
        if (col7 != 0 && col9 != 0 && col10 != 0) {
            appendNode(head, col7, col9, col10);
        }
    }

    input_file.close();  // Close the input file after reading all lines

    auto start = high_resolution_clock::now();  // Start the timer

    // Sort the linked list using mergeSort
    mergeSort(&head);

    auto stop = high_resolution_clock::now();  // Stop the timer
    auto duration = duration_cast<microseconds>(stop - start);  // Calculate the duration

    // Write sorted data to the output file
    Node* current = head;
    while (current) {
        output_file << get<0>(current->data) << "," << get<1>(current->data) << "," << get<2>(current->data) << endl;
        current = current->next;
    }

    output_file.close();  // Close the output file after writing all sorted data

    cout << "Sorting and writing to sorted_size.csv completed" << endl;
    cout << "Time taken: " << duration.count() << " microseconds" << endl;  // Output the duration
}

// Function to merge two sorted linked lists
Node* sortedMerge(Node* a, Node* b) {
    Node* result = nullptr;

    if (!a)
        return b;
    else if (!b)
        return a;

    if (get<0>(a->data) < get<0>(b->data) ||
        (get<0>(a->data) == get<0>(b->data) && get<1>(a->data) < get<1>(b->data)) ||
        (get<0>(a->data) == get<0>(b->data) && get<1>(a->data) == get<1>(b->data) && get<2>(a->data) < get<2>(b->data))) {
        result = a;
        result->next = sortedMerge(a->next, b);
    } else {
        result = b;
        result->next = sortedMerge(a, b->next);
    }

    return result;
}

// Function to split the linked list into two halves
void splitList(Node* source, Node** frontRef, Node** backRef) {
    Node* fast;
    Node* slow;
    slow = source;
    fast = source->next;

    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *frontRef = source;
    *backRef = slow->next;
    slow->next = nullptr;
}

// Merge sort function to sort a linked list
void mergeSort(Node** headRef) {
    Node* head = *headRef;
    Node* a;
    Node* b;

    if (!head || !head->next)
        return;

    splitList(head, &a, &b);

    mergeSort(&a);
    mergeSort(&b);

    *headRef = sortedMerge(a, b);
}

int main() {
    extractFile();  // Call the extractFile function to process the file

    return 0;
}
