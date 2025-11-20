CREATE DATABASE IF NOT EXISTS student_management_system;

USE student_management_system;

CREATE TABLE IF NOT EXISTS students (
	student_id INT PRIMARY KEY AUTO_INCREMENT,
    student_name VARCHAR(100) NOT NULL,
    student_age INT NOT NULL,
    student_department VARCHAR(100) NOT NULL,
    student_gpa DECIMAL(3, 2) NOT NULL
);

DESCRIBE students;
