#include "Student.h"
#include <iostream>
#include <iomanip>
using namespace std;

Student::Student(): student_id(0), student_name(""), student_age(0), student_department(""), student_gpa(0.0) {};

Student::Student(int student_id, string student_name, int student_age, string student_department, float student_gpa) {
    this->student_id = student_id;
    this->student_name = student_name;
    this->student_age = student_age;
    this->student_department = student_department;
    this->student_gpa = student_gpa;
}

Student::Student(string student_name, int student_age, string student_department, float student_gpa) {
    this->student_name = student_name;
    this->student_age = student_age;
    this->student_department = student_department;
    this->student_gpa = student_gpa;
}


int Student::get_student_id() const{
    return this->student_id;
}

string Student::get_student_name() const{
    return this->student_name;
}

int Student::get_student_age() const{
    return this->student_age;
}

string Student::get_student_department() const{
    return this->student_department;
}

float Student::get_student_gpa() const{
    return this->student_gpa;
}


void Student::set_student_id(int student_id){
    this->student_id = student_id;
}

void Student::set_student_name(string student_name){
    this->student_name = student_name;
}

void Student::set_student_age(int student_age){
    this->student_age = student_age;
}

void Student::set_student_department(string student_department){
    this->student_department = student_department;
}

void Student::set_student_gpa(float student_gpa){
    this->student_gpa = student_gpa;
}

void Student::display_student_info() const{
    cout << "ID: " << this->student_id << " | Name: " << this->student_name << " | Age: " << this->student_age
        << " | Department: " << this->student_department << " | GPA: " << this->student_gpa << endl; 
}