# File generated by kdevelop's qmake manager. 
# ------------------------------------------- 
# Subdir relative project main directory: .
# Target is an application:  

DEPENDPATH = . 
INCLUDEPATH += . 
 
QT += xml \
 opengl 
CONFIG += warn_on \
 qt \
 debug \
 opengl 
 \
 
RESOURCES += qalgebra.qrc 
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
 operatorsmodel.h 
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
 operatorsmodel.cpp 

TEMPLATE = app
CONFIG -= release