#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
using namespace std;

class Student {

    private:
        int student_id;
        string student_name;
        int student_age;
        string student_department;
        float student_gpa;

    public:
        Student();
        

        Student(int student_id, string student_name, int student_age, string student_department, float student_gpa);

        Student(string student_name, int student_age, string student_department, float student_gpa);


        int get_student_id() const;
        string get_student_name() const;
        int get_student_age() const;
        string get_student_department() const;
        float get_student_gpa() const;

        void set_student_id(int studnet_id) ;
        void set_student_name(string student_name) ;
        void set_student_age(int student_age);
        void set_student_department(string department) ;
        void set_student_gpa(float gpa) ;


        void display_student_info() const;

};


#endif