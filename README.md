# Student Management System

A C++ application for managing student records with both console and GUI interfaces. The system uses MySQL for data persistence and provides features for adding, viewing, searching, updating, and deleting student records.

## Features

- Add new students with name, age, department, and GPA
- View all students in the database
- Search students by name or department
- Update existing student information
- Delete student records
- Get top students by GPA
- Export student data to CSV format
- Console-based interface
- Qt-based GUI interface

## Prerequisites

### Required Software

- C++ compiler with C++11 support (g++)
- MySQL Server (5.7 or higher)
- MySQL Connector C++ library
- Qt5 (for GUI version)
  - Qt Core
  - Qt GUI
  - Qt Widgets

### Installing Dependencies

#### Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install g++ mysql-server libmysqlcppconn-dev qtbase5-dev qt5-qmake
```

#### MySQL Setup

1. Start MySQL service:
```bash
sudo systemctl start mysql
sudo systemctl enable mysql
```

2. Secure your MySQL installation:
```bash
sudo mysql_secure_installation
```

3. Create the database and table:
```bash
mysql -u root -p < student_db.sql
```

## Database Configuration

The system connects to MySQL using the following default settings:
- Host: `127.0.0.1:3306`
- Database: `student_management_system`
- User: `root`
- Password: Update in `main.cpp` line 20 and `MainWindow.cpp` with your MySQL password

**Important**: Before compiling, update the database password in the source files:
- `main.cpp` line 20
- `MainWindow.cpp` (search for the `connect()` call)

## Building the Project

### Console Version

```bash
g++ -o student_management_system main.cpp Student.cpp StudentManager.cpp StudentValidator.cpp -lmysqlcppconn -std=c++11
```

### GUI Version

```bash
qmake student_management_gui.pro
make
```

## Running the Application

### Console Version

```bash
./student_management_system
```

#### Console Menu Options:
1. Add Student
2. View All Students
3. Search Student
4. Update Student
5. Delete Student
6. Get Top Students
7. Export Students to CSV
8. Exit

### GUI Version

```bash
./student_management_gui
```

The GUI provides a graphical interface with all the same functionality as the console version.

## Project Structure

```
.
├── main.cpp                    # Console application entry point
├── main_gui.cpp               # GUI application entry point
├── Student.h                  # Student class header
├── Student.cpp                # Student class implementation
├── StudentManager.h           # Database manager header
├── StudentManager.cpp         # Database manager implementation
├── StudentValidator.h         # Input validation header
├── StudentValidator.cpp       # Input validation implementation
├── MainWindow.h              # Qt GUI window header
├── MainWindow.cpp            # Qt GUI window implementation
├── mainwindow.ui             # Qt Designer UI file
├── student_management_gui.pro # Qt project file
├── student_db.sql            # Database schema
└── README.md                 # This file
```

## Database Schema

The `students` table contains:
- `student_id` (INT, PRIMARY KEY, AUTO_INCREMENT)
- `student_name` (VARCHAR(100), NOT NULL)
- `student_age` (INT, NOT NULL)
- `student_department` (VARCHAR(100), NOT NULL)
- `student_gpa` (DECIMAL(3,2), NOT NULL)

## Export Functionality

The system can export student records to CSV format. Exported files will be saved in the current directory with the filename you specify.
