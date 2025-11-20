#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include "StudentManager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddStudent();
    void onUpdateStudent();
    void onDeleteStudent();
    void onSearchStudents();
    void onViewAllStudents();
    void onGetTopStudents();
    void onExportToCSV();
    void onTableRowClicked(int row, int column);
    void onRefreshTable();
    void clearInputFields();

private:
    void setupConnections();
    void loadStudentsToTable(const std::vector<Student>& students);

    Ui::MainWindow *ui;
    StudentManager db;
    int selectedStudentId;
};

#endif
