#include <iostream>
#include <limits>
#include "StudentManager.h"
#include "Student.h"
using namespace std;

void clearInputBuffer();
void displayMenu();
void addStudent(StudentManager& db);
void viewAllStudents(StudentManager& db);
void searchStudent(StudentManager& db);
void updateStudent(StudentManager& db);
void deleteStudent(StudentManager& db);
void getTopStudent(StudentManager& db);
void exportToCSV(StudentManager& db);

int main() {

    StudentManager db;
    if (!db.connect("tcp://127.0.0.1:3306", "root", "Umar303081#", "student_management_system")) {
        cout << "Failed to connect to database" << endl;
        return 0;
    }

    int choice;
    while (true) {
        displayMenu();
        cin >> choice;
        clearInputBuffer();

        if (choice == 1) {
            addStudent(db);
        }
        else if (choice == 2) {
            viewAllStudents(db);
        }
        else if (choice == 3) {
            searchStudent(db);
        }
        else if (choice == 4) {
            updateStudent(db);
        }
        else if (choice == 5) {
            deleteStudent(db);
        }
        else if (choice == 6) {
            getTopStudent(db);
        }
        else if (choice == 7) {
            exportToCSV(db);
        }
        else if (choice == 8) {
            break;
        }
        else {
            cout << "Invalid choice" << endl;
        }
    }
    return 0;
}

void clearInputBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void displayMenu() {
    cout << "\n===== STUDENT MANAGEMENT SYSTEM ====" << endl;
    cout << "1. Add Student" << endl;
    cout << "2. View All Students" << endl;
    cout << "3. Search Student" << endl;
    cout << "4. Update Student" << endl;
    cout << "5. Delete Student" << endl;
    cout << "6. Get Top Students" << endl;
    cout << "7. Export Students" << endl;
    cout << "8. Exit" << endl;
    cout << "====================================" << endl;
    cout << "Enter your choice: "; 
}

void addStudent(StudentManager& db) {

    string student_name, student_department;
    int student_age;
    float student_gpa;

    cout << "Enter Name: ";
    getline(cin, student_name);
    cout << "Enter Age: ";
    cin >> student_age;
    cin.ignore();
    cout << "Enter Department: ";
    getline(cin, student_department);
    cout << "Enter GPA: ";
    cin >> student_gpa;

    Student student(student_name, student_age, student_department, student_gpa);
    db.addStudent(student);


}

void viewAllStudents(StudentManager& db) {
    vector<Student> students = db.getAllStudents();

    if (students.empty()) {
        cout << "Database is empty and no student was registerred" << endl;
        return;
    }
    else {
        for (const auto& student: students) {
            student.display_student_info();
        }
    }
}

void searchStudent(StudentManager& db) {
    string keyword;

    cout << "Enter search keyword (name or department): ";
    getline(cin, keyword);

    vector<Student> students = db.searchStudents(keyword);

    if (students.empty()) {
        cout << "No student matches" << endl;
        return;
    }
    else {
        cout << "\n===== SEARCH RESULT =====" << endl;
        for (const auto& student: students) {
            student.display_student_info();
        }
    }

}

void updateStudent(StudentManager& db){
    int student_id;
    cout << "Enter studet ID to update: ";
    cin >> student_id;
    cin.ignore();

    Student student = db.getStudentById(student_id);

    if (student.get_student_id() == 0) {
        cout << "Student is not found" << endl;
        return;
    }
    cout << "\n===== STUDENT DETAILS =====" << endl;
    student.display_student_info();

    string student_name, student_department;
    int student_age;
    float student_gpa;

    cout << "Enter New Name: ";
    getline(cin, student_name);
    cout << "Enter New Age: ";
    cin >> student_age;
    cin.ignore();
    cout << "Enter New Department: ";
    getline(cin, student_department);
    cout << "Enter New GPA: ";
    cin >> student_gpa;


    student.set_student_name(student_name);
    student.set_student_age(student_age);
    student.set_student_department(student_department);
    student.set_student_gpa(student_gpa);

    db.updateStudent(student);
}

void deleteStudent(StudentManager& db) {
    
    int student_id;
    cout << "Enter studet ID to delete: ";
    cin >> student_id;
    cin.ignore();

    Student student = db.getStudentById(student_id);

    if (student.get_student_id() == 0) {
        cout << "Student no found" << endl;
        return;
    }

    cout << "\n===== STUDENT INFO TO BEFORE DELETE =====" << endl;
    student.display_student_info();

    char confirm;
    cout << "Are you sure you want to delete this student? (y/n): ";
    cin >> confirm;
    cin.ignore();

    if (confirm == 'y' || confirm == 'Y') db.deleteStudent(student_id);
    else cout << "Deletion was cancelled" << endl;

}

void getTopStudent(StudentManager& db) {

    int limit;
    cout << "Enter number of top student to view: ";
    cin >> limit;
    cin.ignore();

    vector<Student> students = db.getTopStudents(limit);

    if (students.empty()) {
        cout << "Database is empty and no student was registerred" << endl;
        return;
    }
    else {
        for (const auto& student: students) {
            student.display_student_info();
        }
    }

}

void exportToCSV(StudentManager& db) {

    string filename;
    cout << "Enter the filename: ";
    getline(cin, filename);

    db.exportToCSV(filename);
}