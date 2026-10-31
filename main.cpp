
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>
using namespace std;

struct Student {
    int rollNo;
    string name;
    int age;
    float marks;
};

vector<Student> students;
const string FILE_NAME = "students.txt";

// Get a valid integer
int getInt(string message, int min, int max) {
    int value;
    while (true) {
        cout << message;
        if (cin >> value && value >= min && value <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input! Try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Get valid marks
float getMarks() {
    float marks;
    while (true) {
        cout << "Enter marks (0-100): ";
        if (cin >> marks && marks >= 0 && marks <= 100) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return marks;
        }
        cout << "Invalid marks! Try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Save records to file
void saveData() {
    ofstream file(FILE_NAME);
    for (const auto& s : students) {
        file << s.rollNo << "|" << s.name << "|"
             << s.age << "|" << s.marks << "\n";
    }
}

// Load records from file
void loadData() {
    ifstream file(FILE_NAME);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        Student s;
        string roll, age, marks;

        if (getline(ss, roll, '|') &&
            getline(ss, s.name, '|') &&
            getline(ss, age, '|') &&
            getline(ss, marks)) {
            try {
                s.rollNo = stoi(roll);
                s.age = stoi(age);
                s.marks = stof(marks);
                if (s.rollNo > 0 && !s.name.empty() &&
                    s.age >= 1 && s.age <= 120 &&
                    s.marks >= 0 && s.marks <= 100) {
                    students.push_back(s);
                }
            } catch (...) {
                // Ignore invalid file records
            }
        }
    }
}

// Check duplicate roll number
bool rollExists(int roll) {
    for (const auto& s : students)
        if (s.rollNo == roll) return true;
    return false;
}

// Add student
void addStudent() {
    Student s;

    s.rollNo = getInt("Enter roll number: ", 1, 999999);

    if (rollExists(s.rollNo)) {
        cout << "Roll number already exists!\n";
        return;
    }

    cout << "Enter name: ";
    getline(cin, s.name);

    if (s.name.empty() || s.name.find('|') != string::npos) {
        cout << "Invalid name!\n";
        return;
    }

    s.age = getInt("Enter age: ", 1, 120);
    s.marks = getMarks();

    students.push_back(s);
    saveData();

    cout << "Student added successfully!\n";
}

// Display students
void displayStudents() {
    if (students.empty()) {
        cout << "No student records found.\n";
        return;
    }

    cout << "\n";
    cout << left << setw(12) << "Roll No"
         << setw(25) << "Name"
         << setw(10) << "Age"
         << setw(10) << "Marks" << "\n";

    cout << string(57, '-') << "\n";

    for (const auto& s : students) {
        cout << left << setw(12) << s.rollNo
             << setw(25) << s.name
             << setw(10) << s.age
             << setw(10) << s.marks << "\n";
    }
}

// Search student
void searchStudent() {
    int choice = getInt(
        "Search by 1. Roll Number  2. Name: ", 1, 2);

    bool found = false;

    if (choice == 1) {
        int roll = getInt("Enter roll number: ", 1, 999999);

        for (const auto& s : students) {
            if (s.rollNo == roll) {
                cout << "Found: " << s.name
                     << " | Age: " << s.age
                     << " | Marks: " << s.marks << "\n";
                found = true;
            }
        }
    } else {
        string name;
        cout << "Enter name: ";
        getline(cin, name);

        for (const auto& s : students) {
            if (s.name.find(name) != string::npos) {
                cout << "Found: " << s.rollNo
                     << " | " << s.name
                     << " | Age: " << s.age
                     << " | Marks: " << s.marks << "\n";
                found = true;
            }
        }
    }

    if (!found) cout << "Student not found.\n";
}

// Update student
void updateStudent() {
    int roll = getInt("Enter roll number to update: ", 1, 999999);

    for (auto& s : students) {
        if (s.rollNo == roll) {
            cout << "Current name: " << s.name << "\n";
            cout << "Enter new name: ";
            getline(cin, s.name);

            if (s.name.empty() || s.name.find('|') != string::npos) {
                cout << "Invalid name! Update cancelled.\n";
                return;
            }

            s.age = getInt("Enter new age: ", 1, 120);
            s.marks = getMarks();

            saveData();
            cout << "Student updated successfully!\n";
            return;
        }
    }

    cout << "Student not found.\n";
}

// Delete student
void deleteStudent() {
    int roll = getInt("Enter roll number to delete: ", 1, 999999);

    auto it = find_if(students.begin(), students.end(),
        [roll](const Student& s) {
            return s.rollNo == roll;
        });

    if (it != students.end()) {
        students.erase(it);
        saveData();
        cout << "Student deleted successfully!\n";
    } else {
        cout << "Student not found.\n";
    }
}

// Main menu
int main() {
    loadData();

    int choice;

    do {
        cout << "\n========== STUDENT MANAGEMENT SYSTEM ==========\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "6. Exit\n";
        cout << "===============================================\n";

        choice = getInt("Enter your choice: ", 1, 6);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: cout << "Thank you for using the system!\n"; break;
        }

    } while (choice != 6);

    return 0;
}
