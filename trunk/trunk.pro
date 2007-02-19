# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: .
# Target is an application:  

DEPENDPATH = .
QT += xml opengl gui core
CONFIG += warn_on \
 qt \
 opengl \
 debug \
 console
win32{
    CONFIG -= debug
    CONFIG += release
}

HEADERS += algebra.h \
 analitza.h \
 console.h \
 container.h \
 function.h \
 functionedit.h \
 varedit.h \
 object.h \
 graph3d.h \
 algebrahighlighter.h \
 exp.h \
 expressionedit.h \
 graph2d.h \
 value.h \
 variables.h \
 operator.h  \
 expression.h
SOURCES += algebra.cpp \
 analitza.cpp \
 console.cpp \
 container.cpp \
 function.cpp \
 functionedit.cpp \
 varedit.cpp \
 main.cpp \
 object.cpp \
 graph3d.cpp \
 algebrahighlighter.cpp \
 exp.cpp \
 expressionedit.cpp \
 graph2d.cpp \
 value.cpp \
 variables.cpp \
 operator.cpp  \
 expression.cpp
TEMPLATE = app
TARGET = kalgebra

INCLUDEPATH += .
