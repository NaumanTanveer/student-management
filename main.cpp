#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <unordered_map>
#include <fstream>
using namespace std;

// Structure to store each student's details
struct Student {
    int id;
    string name;
    float gpa;
    Student* next; // For linked list in StudentList
    Student* left; // For BST
    Student* right; // For BST
    bool present;  // for attendance
};

// Queue for maintaining a waitlist for student registration
queue<Student> waitlist;

// Hash Map for fast lookup by student ID
unordered_map<int, Student*> studentMap;

// Binary Search Tree for sorting by GPA
class StudentBST {
public:
    Student* root;

    StudentBST() : root(nullptr) {}

    // Insert student by GPA
    void insert(Student* newStudent) {
        root = insertRec(root, newStudent);

    }

    // Recursive insertion for BST
    Student* insertRec(Student* root, Student* newStudent) {
        if (!root) return newStudent;
        if (newStudent->gpa < root->gpa) {
            root->left = insertRec(root->left, newStudent);
        } else {
            root->right = insertRec(root->right, newStudent);
        }
        return root;
    }

    // In-order traversal to display students sorted by GPA
    void inOrder(Student* root) {
        if (!root) return;
        inOrder(root->left);
        cout << "ID: " << root->id << ", Name: " << root->name << ", GPA: " << root->gpa << "\n";
        inOrder(root->right);
    }

    void displaySorted() {
        if (!root) {
            cout << "No students available.\n";
        }
        else inOrder(root);
    }
};

// Linked list for managing students
class StudentList {
private:
    Student* head;
    int count;
    StudentBST bst; // Make bst a member of StudentList
    const string filename = "students.txt"; // File to store student data

    void getDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        cout << "Date: " << ltm->tm_mday << "/" << ltm->tm_mon + 1 << "/" << ltm->tm_year + 1900;
    }

    void getTime() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        cout << "Time: " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec;
    }

public:
    StudentList() : head(nullptr), count(0) {
        loadFromFile(); // Load student data when program starts
    }

    ~StudentList() {
        saveToFile(); // Save student data when program ends
    }

    // Function to add a student to the list
    void addStudent(int id, string name, float gpa) {
        // Check if student already exists in the hashmap
        if (studentMap.find(id) != studentMap.end()) {
            cout << "Student with ID " << id << " already exists!\n";
            return;
        }

        Student* newStudent = new Student{id, name, gpa, nullptr, nullptr, nullptr};

        // Add student to linked list
        if (!head) {
            head = newStudent;
        } else {
            Student* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newStudent;
        }
        count++;

        // Add student to hash map and BST
        studentMap[id] = newStudent;
        bst.insert(newStudent);
        saveToFile(); // Save updated list to file
        cout << "Student added successfully!\n";
    }

    // Function to display all students in linked list
    void displayStudents() const {
        if (!head) {
            cout << "No students to display.\n";
            return;
        }
        Student* temp = head;
        cout << "ID\tName\tGPA\n";
        while (temp) {
            cout << temp->id << "\t" << temp->name << "\t" << temp->gpa << "\n";
            temp = temp->next;
        }
    }

    // Function to search for a student by ID
    void searchStudent(int id) const {
        if (studentMap.find(id) != studentMap.end()) {
            Student* student = studentMap.at(id);
            cout << "Student Found: ID=" << student->id << ", Name=" << student->name << ", GPA=" << student->gpa << "\n";
        } else {
            cout << "Student with ID " << id << " not found.\n";
        }
    }

    // Function to mark attendance
    void addAttendance(int id) {
        if (studentMap.find(id) != studentMap.end()) {
            Student* student = studentMap.at(id);
            student->present = true;
            cout << "Attendance marked for Student ID " << id << ".\n";
        } else {
            cout << "Student with ID " << id << " not found.\n";
        }
    }



    // Function to add a student to the waitlist
    void addToWaitlist(int id, string name, float gpa) {
        Student newStudent = {id, name, gpa, nullptr, nullptr, nullptr};
        waitlist.push(newStudent);
        cout << "Student added to waitlist.\n";
    }

    // Function to process the waitlist (enroll students if space is available)
    void processWaitlist() {
        while (!waitlist.empty() && count < 10) { // Limiting to 10 students for example
            Student newStudent = waitlist.front();
            waitlist.pop();
            addStudent(newStudent.id, newStudent.name, newStudent.gpa);
        }

        if (!waitlist.empty()) {
            cout << "Waitlist processing complete; some students remain in the waitlist.\n";
        } else {
            cout << "All students in the waitlist have been enrolled.\n";
        }
    }

    // Function to get the total number of students
    int getTotalStudents() const {
        return count;
    }

    // Function to display students sorted by GPA using BST
    void displaySortedByGPA() {
        bst.displaySorted();
    }

    // Save student data to a file
    void saveToFile() const {
        ofstream file(filename);
        if (!file) {
            cerr << "Error saving data to file.\n";
            return;
        }

        Student* temp = head;
        while (temp) {
            file << temp->id << " " << temp->name << " " << temp->gpa << "\n";
            temp = temp->next;
        }
        file.close();
        cout << "Data saved to file successfully.\n";
    }

    // Load student data from a file
    void loadFromFile() {
        ifstream file(filename);
        if (!file) {
            cout << "No previous data found.\n";
            return;
        }

        int id;
        string name;
        float gpa;
        while (file >> id) {
            file.ignore(); // To ignore whitespace
            getline(file, name, ' ');
            file >> gpa;
            addStudent(id, name, gpa);
        }
        file.close();
        cout << "Data loaded from file successfully.\n";
    }

    void startUpMenu() {
        cout << "\n\n\n";
        cout << "\t\t\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2"<<setfill(' ')<<setw(86)<<"\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2\t"; getDate(); cout<<setfill(' ')<<setw(69) << "\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2\t"; getTime(); cout<<setfill(' ')<<setw(71) << "\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2"<<setfill(' ')<<setw(86)<<"\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2"<<setfill(' ')<<setw(86)<<"\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2"<<setfill(' ')<<setw(46)<<"Welcome to"<<setfill(' ')<<setw(40)<<"\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2"<<setfill(' ')<<setw(50)<<" Student  Management"<<setfill(' ')<<setw(36)<<"\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2"<<setfill(' ')<<setw(44)<<"System"<<setfill(' ')<<setw(42)<<"\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2"<<setfill(' ')<<setw(86)<<"\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2"<<setfill(' ')<<setw(86)<<"\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2"<<setfill(' ')<<setw(86)<<"\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2"<<setfill(' ')<<setw(86)<<"\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2"<<setfill(' ')<<setw(86)<<"\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2\t\tDeveloped By :"<<setfill(' ')<<setw(66)<<"\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2"<<setfill(' ')<<setw(64)<<"Nauman Tanveer"<<setfill(' ')<<setw(22)<<"\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2"<<setfill(' ')<<setw(86)<<"\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2"<<setfill(' ')<<setw(86)<<"\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2"<<setfill(' ')<<setw(86)<<"\xB2\xB2" << endl;
        cout << "\t\t\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2" << endl;
        cout << "\n\n\n\t\tPress Any Key to Enter... ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        cout << endl;
        system("clear");
    }
};

int main() {
    StudentList studentList;
    int choice, id;
    string name;
    float gpa;

    studentList.startUpMenu();

    while (true) {
        system("clear");

        do {
            cout << "\n\n\t\t\t____________________________________________________________________________________________________________________________\n";
            cout << "\t\t\t\t\t\t\t\t\t\t       ____________________________" << endl;
            cout << "\t\t\t\t\t\t\t\t\t\t       \xB2                          \xB2\n";
            cout << "\t\t\t\t\t\t\t\t\t\t       \xB2       ~ Main Menu ~      \xB2" << endl;
            cout << "\t\t\t\t\t\t\t\t\t\t       \xB2__________________________\xB2" << endl;
            cout << "\n";

            // Line 1: Add and Display Students
            cout << "\t\t\t__________________________________________________\t\t\t___________________________________________________" << endl;
            cout << "\t\t\t\xB2                                                \xB2\t\t\t\xB2                                                 \xB2" << endl;
            cout << "\t\t\t\xB2        Press 1 -> to Add Student               \xB2\t\t\t\xB2        Press 2 -> to Display All Students       \xB2" << endl;
            cout << "\t\t\t\xB2________________________________________________\xB2\t\t\t\xB2_________________________________________________\xB2" << endl;
            cout << "\n";

            // Line 2: Search and Delete Students
            cout << "\t\t\t__________________________________________________\t\t\t___________________________________________________" << endl;
            cout << "\t\t\t\xB2                                                \xB2\t\t\t\xB2                                                 \xB2" << endl;
            cout << "\t\t\t\xB2        Press 3 -> to Search Student by ID      \xB2\t\t\t\xB2        Press 4 -> Add Attendance                \xB2" << endl;
            cout << "\t\t\t\xB2________________________________________________\xB2\t\t\t\xB2_________________________________________________\xB2" << endl;
            cout << "\n";

            // Line 3: Undo Delete and Waitlist
            cout << "\t\t\t__________________________________________________\t\t\t___________________________________________________" << endl;
            cout << "\t\t\t\xB2                                                \xB2\t\t\t\xB2                                                 \xB2" << endl;
            cout << "\t\t\t\xB2        Press 5 ->to Add Student to Waitlist    \xB2\t\t\t\xB2        Press 6 -> to Process Waitlist           \xB2" << endl;
            cout << "\t\t\t\xB2________________________________________________\xB2\t\t\t\xB2_________________________________________________\xB2" << endl;
            cout << "\n";

            // Line 4: Process Waitlist and Total Students
            cout << "\t\t\t__________________________________________________\t\t\t___________________________________________________" << endl;
            cout << "\t\t\t\xB2                                                \xB2\t\t\t\xB2                                                 \xB2" << endl;
            cout << "\t\t\t\xB2        Press 7 -> to Display Students by GPA   \xB2\t\t\t\xB2        Press 8 -> to Total Number of Students   \xB2" << endl;
            cout << "\t\t\t\xB2________________________________________________\xB2\t\t\t\xB2_________________________________________________\xB2" << endl;
            cout << "\n";

            // Line 5: Sort by GPA and Exit
            cout << "\t\t\t__________________________________________________" << endl;
            cout << "\t\t\t\xB2                                                \xB2" << endl;
            cout << "\t\t\t\xB2       Press 0 -> to Exit                       \xB2" << endl;
            cout << "\t\t\t\xB2________________________________________________\xB2" << endl;
            cout << "\n\n\t\t\t____________________________________________________________________________________________________________________________\n";
            cout << "\n\n\t\t\tEnter Your Choice = ";
            cin >> choice;

        } while (choice < 0 || choice > 8);
        switch (choice) {
            case 1:
                cout << "Enter Student ID: ";
                cin >> id;
                cout << "Enter Student Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Student GPA: ";
                cin >> gpa;
                studentList.addStudent(id, name, gpa);
                break;
            case 2:
                studentList.displayStudents();
                break;
            case 3:
                cout << "Enter Student ID to search: ";
                cin >> id;
                studentList.searchStudent(id);
                break;
            case 4:
                cout << "Enter Student ID: ";
                cin >> id;
                studentList.addAttendance(id);
                break;
            case 5:
                cout << "Enter Student ID: ";
                cin >> id;
                cout << "Enter Student Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Student GPA: ";
                cin >> gpa;
                studentList.addToWaitlist(id, name, gpa);
                break;
            case 6:
                studentList.processWaitlist();
                break;
            case 7:
                studentList.displaySortedByGPA();
                break;
            case 8:
                cout << "Total Students: " << studentList.getTotalStudents() << "\n";
                break;
            case 0:
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice! Please try again.\n";
                break;
        }
    }

    return 0;
}

