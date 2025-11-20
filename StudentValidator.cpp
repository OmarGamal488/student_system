#include "StudentValidator.h"
#include <cctype>

bool StudentValidator::isValidName(const string& name) {
    if (name.empty() || name.length() > 100) {
        return false;
    }
    return true;
}

bool StudentValidator::isValidAge(const int& age) {
    if (age < 17 || age > 100) {
        return false;
    }
    return true;
}

bool StudentValidator::isValidDepartment(const string& department) {
    if (department.empty() || department.length() > 100) {
        return false;
    }
    return true;
}

bool StudentValidator::isValidGPA(const float& gpa) {
    if (gpa < 0.0f || gpa > 4.0f) {
        return false;
    }
    return true;
}

bool StudentValidator::isValidStudent(const string& name, const int& age, const string& department, const float& gpa) {
    return isValidName(name) && isValidAge(age) && isValidDepartment(department) && isValidGPA(gpa);
}