QT += core gui widgets

TARGET = student_management_gui
TEMPLATE = app

CONFIG += c++11

SOURCES += \
    main_gui.cpp \
    MainWindow.cpp \
    Student.cpp \
    StudentManager.cpp \
    StudentValidator.cpp

HEADERS += \
    MainWindow.h \
    Student.h \
    StudentManager.h \
    StudentValidator.h

FORMS += \
    mainwindow.ui

# MySQL Connector C++ Configuration
INCLUDEPATH += /usr/include/jdbc
INCLUDEPATH += /usr/include/cppconn

LIBS += -lmysqlcppconn

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
