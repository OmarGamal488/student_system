#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "StudentValidator.h"
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), selectedStudentId(-1) {

    ui->setupUi(this);

    if (!db.connect("tcp://127.0.0.1:3306", "root", "Umar303081#", "student_management_system")) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to database");
    }

    ui->studentTable->horizontalHeader()->setStretchLastSection(true);
    setupConnections();
    onViewAllStudents();
}

MainWindow::~MainWindow() {
    db.disconnect();
    delete ui;
}

void MainWindow::setupConnections() {
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddStudent);
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::onUpdateStudent);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteStudent);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::onSearchStudents);
    connect(ui->viewAllButton, &QPushButton::clicked, this, &MainWindow::onViewAllStudents);
    connect(ui->topStudentsButton, &QPushButton::clicked, this, &MainWindow::onGetTopStudents);
    connect(ui->exportButton, &QPushButton::clicked, this, &MainWindow::onExportToCSV);
    connect(ui->clearButton, &QPushButton::clicked, this, &MainWindow::clearInputFields);
    connect(ui->refreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshTable);
    connect(ui->studentTable, &QTableWidget::cellClicked, this, &MainWindow::onTableRowClicked);
}

void MainWindow::loadStudentsToTable(const std::vector<Student>& students) {
    ui->studentTable->setRowCount(0);

    for (const auto& student : students) {
        int row = ui->studentTable->rowCount();
        ui->studentTable->insertRow(row);

        ui->studentTable->setItem(row, 0, new QTableWidgetItem(QString::number(student.get_student_id())));
        ui->studentTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(student.get_student_name())));
        ui->studentTable->setItem(row, 2, new QTableWidgetItem(QString::number(student.get_student_age())));
        ui->studentTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(student.get_student_department())));
        ui->studentTable->setItem(row, 4, new QTableWidgetItem(QString::number(student.get_student_gpa(), 'f', 2)));
    }

    ui->studentTable->resizeColumnsToContents();
    ui->statusLabel->setText(QString("Loaded %1 student(s)").arg(students.size()));
}

void MainWindow::clearInputFields() {
    ui->nameInput->clear();
    ui->ageInput->setValue(18);
    ui->departmentInput->clear();
    ui->gpaInput->setValue(0.0);
    selectedStudentId = -1;
    ui->studentTable->clearSelection();
    ui->statusLabel->setText("Fields cleared");
}

void MainWindow::onAddStudent() {
    QString name = ui->nameInput->text().trimmed();
    QString department = ui->departmentInput->text().trimmed();
    int age = ui->ageInput->value();
    float gpa = ui->gpaInput->value();

    bool isValid = StudentValidator::isValidStudent(
        name.toStdString(),
        age,
        department.toStdString(),
        gpa
    );

    if (!isValid) {
        QMessageBox::warning(this, "Input Error", "Please enter valid student data");
        return;
    }
    Student student(name.toStdString(), age, department.toStdString(), gpa);

    if (db.addStudent(student)) {
        QMessageBox::information(this, "Success", "Student added successfully");
        clearInputFields();
        onRefreshTable();
    } else {
        QMessageBox::critical(this, "Error", "Failed to add student");
    }
}

void MainWindow::onUpdateStudent() {
    if (selectedStudentId == -1) {
        QMessageBox::warning(this, "Selection Error", "Please select a student from the table");
        return;
    }

    QString name = ui->nameInput->text().trimmed();
    QString department = ui->departmentInput->text().trimmed();
    int age = ui->ageInput->value();
    float gpa = ui->gpaInput->value();

    bool isValid = StudentValidator::isValidStudent(
        name.toStdString(),
        age,
        department.toStdString(),
        gpa
    );

    if (!isValid) {
        QMessageBox::warning(this, "Input Error", "Please enter valid student data");
        return;
    }

    Student student(selectedStudentId, name.toStdString(), age, department.toStdString(), gpa);

    if (db.updateStudent(student)) {
        QMessageBox::information(this, "Success", "Student updated successfully");
        clearInputFields();
        onRefreshTable();
    } else {
        QMessageBox::critical(this, "Error", "Failed to update student");
    }
}

void MainWindow::onDeleteStudent() {
    if (selectedStudentId == -1) {
        QMessageBox::warning(this, "Selection Error", "Please select a student from the table");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Delete",
                                   "Are you sure you want to delete this student?",
                                   QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (db.deleteStudent(selectedStudentId)) {
            QMessageBox::information(this, "Success", "Student deleted successfully");
            clearInputFields();
            onRefreshTable();
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete student");
        }
    }
}

void MainWindow::onSearchStudents() {
    QString keyword = ui->searchInput->text().trimmed();

    if (keyword.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter search keyword");
        return;
    }

    std::vector<Student> students = db.searchStudents(keyword.toStdString());

    if (students.empty()) {
        QMessageBox::information(this, "Search Results", "No students found");
        ui->studentTable->setRowCount(0);
    } else {
        loadStudentsToTable(students);
    }
}

void MainWindow::onViewAllStudents() {
    std::vector<Student> students = db.getAllStudents();
    loadStudentsToTable(students);
}

void MainWindow::onGetTopStudents() {
    int limit = ui->topStudentsInput->value();
    std::vector<Student> students = db.getTopStudents(limit);

    if (students.empty()) {
        QMessageBox::information(this, "Top Students", "No students found");
        ui->studentTable->setRowCount(0);
    } else {
        loadStudentsToTable(students);
        ui->statusLabel->setText(QString("Showing top %1 student(s)").arg(students.size()));
    }
}

void MainWindow::onExportToCSV() {
    QString filename = ui->exportFilenameInput->text().trimmed();

    if (filename.isEmpty()) {
        filename = QFileDialog::getSaveFileName(this, "Export to CSV", "", "CSV Files (*.csv)");
        if (filename.isEmpty()) {
            return;
        }
    }

    if (db.exportToCSV(filename.toStdString())) {
        QMessageBox::information(this, "Success", QString("Data exported to %1").arg(filename));
        ui->statusLabel->setText(QString("Exported to %1").arg(filename));
    } else {
        QMessageBox::critical(this, "Error", "Failed to export data");
    }
}

void MainWindow::onTableRowClicked(int row, int column) {
    QTableWidgetItem *idItem = ui->studentTable->item(row, 0);
    if (idItem) {
        selectedStudentId = idItem->text().toInt();

        ui->nameInput->setText(ui->studentTable->item(row, 1)->text());
        ui->ageInput->setValue(ui->studentTable->item(row, 2)->text().toInt());
        ui->departmentInput->setText(ui->studentTable->item(row, 3)->text());
        ui->gpaInput->setValue(ui->studentTable->item(row, 4)->text().toDouble());

        ui->statusLabel->setText(QString("Selected student ID: %1").arg(selectedStudentId));
    }
}

void MainWindow::onRefreshTable() {
    onViewAllStudents();
}
