# set the include path for X, qt and KDE
INCLUDES = $(all_includes)

# these are the headers for your project
noinst_HEADERS = kalgebra.h function.h qdoublepoint.h kfunctionedit.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/kalgebra.pot

KDE_ICON = kalgebra

#########################################################################
# APPLICATION SECTION
#########################################################################
# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = kalgebra

# the application source, library search path, and link libraries
kalgebra_SOURCES = main.cpp kalgebra.cpp analitza.cpp analitza.h q3dgraph.cpp \
				q3dgraph.h qgraph.cpp qgraph.h qexp.cpp qexp.h function.cpp qdoublepoint.cpp \
	kfunctionedit.cpp
kalgebra_LDFLAGS = $(KDE_RPATH) $(all_libraries)
kalgebra_LDADD = $(LIB_KDEUI) /usr/lib/libkhtml.so /usr/lib/libGLU.so

# this is where the desktop file will go 
shelldesktopdir   = $(kde_appsdir)/Utilities
shelldesktop_DATA = kalgebra.desktop

# this is where the shell's XML-GUI resource file goes
shellrcdir   = $(kde_datadir)/kalgebra
shellrc_DATA = kalgebraui.rc

