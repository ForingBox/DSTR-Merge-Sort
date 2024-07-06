#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>

using namespace std;
using namespace std::chrono;

// define a node class to represent each element in the linked list
struct node {
public:
    int room, bathroom, size; // attributes to store the room, bathroom, and size information
    node* next; // pointer to the next node in the list

    // constructor to initialize a new node
    node(int room, int bathroom, int size) : room(room), bathroom(bathroom), size(size), next(nullptr) {}
};

// define a linked list class to manage the list of nodes
struct llist {
private:
    node* head; // pointer to the head of the linked list

    // function to merge two sorted linked lists
    node* SortedMerge(node* first, node* second) {
        if (first == nullptr) return second; // if first list is empty, return second list
        else if (second == nullptr) return first; // if second list is empty, return first list

        node* result = nullptr;
        // compare the nodes of both lists and merge them in sorted order
        if (first->room < second->room || 
            (first->room == second->room && first->bathroom < second->bathroom) ||
            (first->room == second->room && first->bathroom == second->bathroom && first->size < second->size)) {
            result = first;
            result->next = SortedMerge(first->next, second);
        } else {
            result = second;
            result->next = SortedMerge(first, second->next);
        }
        return result;
    }

    // function to split the linked list into two halves
    void splitList(node* source, node** firstRef, node** secondRef) {
        node* slow = source;
        node* fast = source->next;
        // use the fast and slow pointer technique to find the middle of the list
        while (fast != nullptr) {
            fast = fast->next;
            if (fast != nullptr) {
                slow = slow->next;
                fast = fast->next;
            }
        }
        *firstRef = source;
        *secondRef = slow->next;
        slow->next = nullptr; // split the list into two halves
    }

    // recursive merge sort function for linked list
    void MergeSort(node** headRef) {
        node* head = *headRef;
        if ((head == nullptr) || (head->next == nullptr)) return; // base case: if list is empty or has one element

        node* first = nullptr;
        node* second = nullptr;
        splitList(head, &first, &second); // split the list into two halves

        MergeSort(&first); // recursively sort the first half
        MergeSort(&second); // recursively sort the second half

        *headRef = SortedMerge(first, second); // merge the two sorted halves
    }

public:
    llist() : head(nullptr) {} // constructor to initialize an empty linked list

    // function to insert a new node at the end of the linked list
    void insert(int room, int bathroom, int size) {
        node* newnode = new node(room, bathroom, size);
        if (head == nullptr) {
            head = newnode; // if the list is empty, make the new node the head
        } else {
            node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newnode; // append the new node at the end of the list
        }
    }

    // function to sort the linked list
    void sort() {
        MergeSort(&head); // call the merge sort function on the head of the list
    }

    // function to write the sorted linked list to a csv file
    void writeToCSV(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "failed to open file: " << filename << endl;
            return;
        }

        node* current = head;
        while (current != nullptr) {
            file << current->room << "," << current->bathroom << "," << current->size << endl;
            current = current->next;
        }
        file.close();
    }

    // destructor to free the memory allocated for the linked list
    ~llist() {
        node* current = head;
        while (current != nullptr) {
            node* next = current->next;
            delete current;
            current = next;
        }
    }
};

// function to read data from a csv file and insert it into a linked list
void csvIntoList(const string& filename, llist& list) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "failed to open file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;

        int columnNumber = 1;
        int room = 0, bathroom = 0, size = 0;

        // read each cell in the line
        while (getline(ss, cell, ',')) {
            // extract the values for room, bathroom, and size from the appropriate columns
            if (columnNumber == 7) {
                try {
                    room = stoi(cell);
                } catch (const invalid_argument& e) {
                    cerr << "invalid integer value in column 7: " << cell << endl;
                } catch (const out_of_range& e) {
                    cerr << "integer value out of range in column 7: " << cell << endl;
                }
            } else if (columnNumber == 9) {
                try {
                    bathroom = stoi(cell);
                } catch (const invalid_argument& e) {
                    cerr << "invalid integer value in column 9: " << cell << endl;
                } catch (const out_of_range& e) {
                    cerr << "integer value out of range in column 9: " << cell << endl;
                }
            } else if (columnNumber == 10) {
                try {
                    size = stoi(cell);
                } catch (const invalid_argument& e) {
                    cerr << "invalid integer value in column 10: " << cell << endl;
                } catch (const out_of_range& e) {
                    cerr << "integer value out of range in column 10: " << cell << endl;
                }
            }
            columnNumber++;
        }

        // insert the extracted values into the linked list if they are valid
        if (columnNumber >= 10) {
            list.insert(room, bathroom, size);
        }
    }
    file.close();
}

int main() {
    llist newList; // create a linked list
    csvIntoList("mudah-apartment-kl-selangor mmz.csv", newList); // read data from csv and insert into linked list

    auto start = high_resolution_clock::now();

    newList.sort(); // sort the linked list

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "time taken for sorting: " << duration.count() << " microseconds" << endl;

    newList.writeToCSV("sorted_size.csv"); // write the sorted linked list to a csv file

    return 0;
}
