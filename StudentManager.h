#ifndef StudentManager_H
#define StudentManager_H

#include <mysql-cppconn/jdbc/mysql_driver.h>
#include <mysql-cppconn/jdbc/mysql_connection.h>
#include <mysql-cppconn/jdbc/cppconn/statement.h>
#include <mysql-cppconn/jdbc/cppconn/prepared_statement.h>
#include <mysql-cppconn/jdbc/cppconn/resultset.h>
#include <vector>
#include "Student.h"


class StudentManager {

    private:
        sql::mysql::MySQL_Driver* driver;
        sql::Connection* con;

    public:
        StudentManager();
        ~StudentManager();

        bool connect(const string& host, const string& user, const string& password, const string& database);
        void disconnect();

        bool addStudent(const Student& student);
        vector<Student> getAllStudents();
        Student getStudentById(int student_id);
        bool updateStudent(const Student& student);
        bool deleteStudent(int student_id);
        vector<Student> searchStudents(const string& keyword);
        vector<Student> getTopStudents(const int limit);
        bool exportToCSV(const string& filename);
};


#endif