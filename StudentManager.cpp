#include "StudentManager.h"
#include "StudentValidator.h"
#include <iostream>
#include <mysql-cppconn/jdbc/cppconn/exception.h>
#include <fstream>
#include <iomanip>

using namespace std;

StudentManager::StudentManager() : driver(NULL), con(NULL) {};

StudentManager::~StudentManager() {
    disconnect();
};

bool StudentManager::connect(const string& host, const string& user, const string& password, const string& database) {

    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(host, user, password);
        con->setSchema(database);

        cout << "Database connected succesfully" << endl;
        return true;
    }
    catch (sql::SQLException& e) {
        cout << "Database connection failed: " <<e.what() << endl;
        return false; 
    }
}

void StudentManager::disconnect() {
    if (this->con) {
        delete con;
        con = NULL;
    }
}

bool StudentManager::addStudent(const Student& student) {

    try {

        bool isValid = StudentValidator().isValidStudent(
            student.get_student_name(),
            student.get_student_age(),
            student.get_student_department(),
            student.get_student_gpa()
        );

        if (!isValid) {
            cout << "Invalid student data. Please check the input values." << endl;
            return false;
        }

        sql::PreparedStatement* sql_statement = con->prepareStatement(
            "INSERT INTO students (student_name, student_age, student_department, student_gpa) VALUES (?, ?, ?, ?)" 
        );

        sql_statement->setString(1, student.get_student_name());
        sql_statement->setInt(2, student.get_student_age());
        sql_statement->setString(3, student.get_student_department());
        sql_statement->setDouble(4, student.get_student_gpa());
        sql_statement->execute();

        delete sql_statement;
        cout << "Student added successfully" << endl;
        return true;

    }
    catch (sql::SQLException& e) {
        cout << "Error adding student: " << e.what() << endl;
        return false;
    }
}

vector<Student> StudentManager::getAllStudents() {

    vector<Student> students;
    try {
        sql::Statement* statement = con->createStatement();
        sql::ResultSet* result = statement->executeQuery("SELECT * FROM students");

        while (result->next()) {
            Student s(
                result->getInt("student_id"),
                result->getString("student_name"),
                result->getInt("student_age"),
                result->getString("student_department"),
                result->getDouble("student_gpa")
            );
            students.push_back(s);
        }
        delete result;
        delete statement;
    }
    catch (sql::SQLException& e) {
        cout << "Error retrieving students: " << e.what() << endl;
    }

    return students;
}

Student StudentManager::getStudentById(int student_id) {

    Student student;
    try {
        sql::PreparedStatement* sql_statement = con->prepareStatement(
            "SELECT * FROM students WHERE student_id = ?"
        );
        sql_statement->setInt(1, student_id);
        
        sql::ResultSet* result = sql_statement->executeQuery();
        while (result->next()) {
            student = Student (
                result->getInt("student_id"),
                result->getString("student_name"),
                result->getInt("student_age"),
                result->getString("student_department"),
                result->getDouble("student_gpa")
            );
        }

        delete result;
        delete sql_statement;
    }
    catch (sql::SQLException& e) {
        cout << "Error retrieving student: " << e.what() << endl;
    }
    return student;
}

bool StudentManager::updateStudent(const Student& student) {

    try{

        bool isValid = StudentValidator().isValidStudent(
            student.get_student_name(),
            student.get_student_age(),
            student.get_student_department(),
            student.get_student_gpa()
        );

        if (!isValid) {
            cout << "Invalid student data. Please check the input values." << endl;
            return false;
        }
        
        sql::PreparedStatement*  sql_statement = con->prepareStatement(
            "UPDATE students SET student_name = ?, student_age = ?, student_department = ?, student_gpa = ? WHERE student_id = ?"
        );
        
        sql_statement->setString(1, student.get_student_name());
        sql_statement->setInt(2, student.get_student_age());
        sql_statement->setString(3, student.get_student_department());
        sql_statement->setDouble(4, student.get_student_gpa());
        sql_statement->setInt(5, student.get_student_id());

        sql_statement->executeUpdate();
        delete sql_statement;
        cout << "Student updated successfully" << endl;
        return true;

    }
    catch(sql::SQLException& e) {
        cout << "Error updating student: " << e.what() << endl;
        return false;
    }
}

bool StudentManager::deleteStudent(int student_id) {

    try {
        sql::PreparedStatement* sql_statement = con->prepareStatement(
            "DELETE FROM students WHERE student_id = ?"
        );

        sql_statement->setInt(1, student_id);
        sql_statement->executeUpdate();
        delete sql_statement;

        cout << "Student deleted successfully" << endl;
        return true;
    }
    catch (sql::SQLException& e) {
        cout << "Error deleting student: " << e.what() << endl;
        return false;
    }
}

vector<Student> StudentManager::searchStudents(const string& keyword) {

    vector<Student> students;
    try {
        sql::PreparedStatement* sql_statement = con->prepareStatement(
            "SELECT * FROM students WHERE student_name LIKE ? OR student_department LIKE ?"
        );

        string searchPattern = "%" + keyword + "%";
        sql_statement->setString(1, searchPattern);
        sql_statement->setString(2, searchPattern);

        sql::ResultSet* result = sql_statement->executeQuery();

        while (result->next()) {
            Student s(
                result->getInt("student_id"),
                result->getString("student_name"),
                result->getInt("student_age"),
                result->getString("student_department"),
                result->getDouble("student_gpa")
            );

            students.push_back(s);
        }

        delete result;
        delete sql_statement;
    }
    catch (sql::SQLException& e){
        cout << "Error searching students: " << e.what() << endl;
    }

    return students;
}

vector<Student> StudentManager::getTopStudents(const int limit){
    
    vector<Student> students;
    try {

        sql::PreparedStatement* sql_statement = con->prepareStatement (
            "SELECT * FROM students ORDER BY student_gpa DESC LIMIT ?"
        );

        sql_statement->setInt(1, limit);

        sql::ResultSet* result = sql_statement->executeQuery();

        while (result->next()) {
            Student s(
                result->getInt("student_id"),
                result->getString("student_name"),
                result->getInt("student_age"),
                result->getString("student_department"),
                result->getDouble("student_gpa")
            );

            students.push_back(s);
        }

        delete result;
        delete sql_statement;
    }
    catch (sql::SQLException& e){
        cout << "Error searching students: " << e.what() << endl;
    }

    return students;
}

bool StudentManager::exportToCSV(const string& filename) {

    try {

        string csv_file = filename + ".csv";
        ofstream file(csv_file);

        if (!file.is_open()) {

            cout << "Error: Could not create file" << filename << endl;
            return false;
        }

        file << "student_id, student_name, student_age, student_department, student_gpa\n";

        vector<Student> students = getAllStudents();

        for (const auto& student: students) {

            file << student.get_student_id() << ", "
                << student.get_student_name() << ", "
                << student.get_student_age() << ", "
                << student.get_student_department() << ", "
                << student.get_student_gpa() << "\n";
        }

        file.close();
        cout << "\nSuccessfully exported " << students.size() << " students to " << filename << endl;
        return true;

    }
    catch (exception& e) {
        cout << "Error exporting to CSV: " << e.what() << endl;
        return false;
    }
}