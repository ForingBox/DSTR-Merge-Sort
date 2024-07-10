#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;

/* Link list node */
class node {
public:
    int data1, data2, data3;
    node* next;
    node(int data1, int data2, int data3) : data1(data1), data2(data2), data3(data3), next(nullptr) {}
};

class llist {
private:
    node* head;

    node* getTail(node* head) {
        while (head != nullptr && head->next != nullptr)
            head = head->next;
        return head;
    }

    node* partition(node* head, node* end, node** newHead, node** newEnd) {
        node* pivot = end;
        node* prev = nullptr, * curr = head, * tail = pivot;

        while (curr != pivot) {
            if ((curr->data1 < pivot->data1) || 
                (curr->data1 == pivot->data1 && curr->data2 < pivot->data2) || 
                (curr->data1 == pivot->data1 && curr->data2 == pivot->data2 && curr->data3 < pivot->data3)) {
                if ((*newHead) == nullptr)
                    (*newHead) = curr;

                prev = curr;
                curr = curr->next;
            } else {
                if (prev)
                    prev->next = curr->next;

                node* temp = curr->next;
                curr->next = nullptr;
                tail->next = curr;
                tail = curr;
                curr = temp;
            }
        }

        if ((*newHead) == nullptr)
            (*newHead) = pivot;

        (*newEnd) = tail;

        return pivot;
    }

    node* quickSortRecur(node* head, node* end) {
        if (!head || head == end)
            return head;

        node* newHead = nullptr, * newEnd = nullptr;

        node* pivot = partition(head, end, &newHead, &newEnd);

        if (newHead != pivot) {
            node* temp = newHead;
            while (temp->next != pivot)
                temp = temp->next;
            temp->next = nullptr;

            newHead = quickSortRecur(newHead, temp);
            temp = getTail(newHead);
            temp->next = pivot;
        }

        pivot->next = quickSortRecur(pivot->next, newEnd);

        return newHead;
    }

public:
    llist() : head(nullptr) {}

    void insert(int data1, int data2, int data3) {
        node* newnode = new node(data1, data2, data3);
        if (head == nullptr) {
            head = newnode;
        } else {
            node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newnode;
        }
    }

    void display() {
        node* current = head;
        cout << "Start -> ";
        while (current != nullptr) {
            cout << "(" << current->data1 << ", " << current->data2 << ", " << current->data3 << ") -> ";
            current = current->next;
        }
        cout << "End" << endl;
    }

    void sort() {
        head = quickSortRecur(head, getTail(head));
    }

    void writeToCSV(const string& filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            return;
        }

        node* current = head;
        while (current != nullptr) {
            file << current->data1 << "," << current->data2 << "," << current->data3 << endl;
            current = current->next;
        }
        file.close();
    }

    ~llist() {
        node* current = head;
        while (current != nullptr) {
            node* next = current->next;
            delete current;
            current = next;
        }
    }
};

void csvIntoList(const string& filename, llist& list) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;

        int columnNumber = 1;
        int data1 = 0, data2 = 0, data3 = 0;

        while (getline(ss, cell, ',')) {
            if (columnNumber == 7) {
                try {
                    data1 = stoi(cell);
                } catch (const invalid_argument& e) {
                    cerr << "Invalid integer value in column 7: " << cell << endl;
                } catch (const out_of_range& e) {
                    cerr << "Integer value out of range in column 7: " << cell << endl;
                }
            } else if (columnNumber == 9) {
                try {
                    data2 = stoi(cell);
                } catch (const invalid_argument& e) {
                    cerr << "Invalid integer value in column 9: " << cell << endl;
                } catch (const out_of_range& e) {
                    cerr << "Integer value out of range in column 9: " << cell << endl;
                }
            } else if (columnNumber == 10) {
                try {
                    data3 = stoi(cell);
                } catch (const invalid_argument& e) {
                    cerr << "Invalid integer value in column 10: " << cell << endl;
                } catch (const out_of_range& e) {
                    cerr << "Integer value out of range in column 10: " << cell << endl;
                }
            }
            columnNumber++;
        }

        if (columnNumber >= 10) {
            list.insert(data1, data2, data3);
        }
    }
    file.close();
}

int main() {
    llist newList;
    csvIntoList("mudah-apartment-kl-selangor mmz.csv", newList);

    cout << "Original list: ";
    //newList.display();

    auto start = high_resolution_clock::now();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken for sorting: " << duration.count() << " microseconds" << endl;

    newList.writeToCSV("sorted_size.csv");

    return 0;
}
