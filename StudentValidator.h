#ifndef STUDENTVALIDATOR_H
#define STUDENTVALIDATOR_H

#include <string>
#include <vector>

using namespace std;

class StudentValidator {

    public:
        static bool isValidName(const string& name);
        static bool isValidAge(const int& age);
        static bool isValidDepartment(const string& department);
        static bool isValidGPA(const float& gpa);
        static bool isValidStudent(const string& name, const int& age, const string& department, const float& gpa);
};

#endif