# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: .
# Target is an application:  

DEPENDPATH = .
QT += xml \
 opengl 
CONFIG += warn_on \
 qt \
 opengl \
 debug
win32{    #Qt official release doesn't bundle debug libs I think
    CONFIG -= debug
    CONFIG += release staticlib
}

HEADERS += algebra.h \
 analitza.h \
 console.h \
 container.h \
 function.h \
 kfunctionedit.h \
 kvaredit.h \
 object.h \
 q3dgraph.h \
 qalgebrahighlighter.h \
 qexp.h \
 qexpressionedit.h \
 qgraph.h \
 value.h \
 variables.h \
 operator.h 
SOURCES += algebra.cpp \
 analitza.cpp \
 console.cpp \
 container.cpp \
 function.cpp \
 kfunctionedit.cpp \
 kvaredit.cpp \
 main.cpp \
 object.cpp \
 q3dgraph.cpp \
 qalgebrahighlighter.cpp \
 qexp.cpp \
 qexpressionedit.cpp \
 qgraph.cpp \
 value.cpp \
 variables.cpp \
 operator.cpp 

TEMPLATE = app
INCLUDEPATH += .

