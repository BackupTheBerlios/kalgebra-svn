project(kalgebra)

find_package(KDE4 REQUIRED)
include_directories( ${CMAKE_SOURCE_DIR} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} )

set(kalgebra_SRCS
	expression.cpp
	algebra.cpp
	analitza.cpp
	console.cpp
	container.cpp
	function.cpp
	functionedit.cpp
	varedit.cpp
	main.cpp
	object.cpp
	operator.cpp
	graph3d.cpp
	algebrahighlighter.cpp
	exp.cpp
	expressionedit.cpp
	graph2d.cpp
	value.cpp
	variables.cpp
)

kde4_automoc( ${kalgebra_SRCS} )
kde4_add_executable(kalgebra ${kalgebra_SRCS} )
target_link_libraries(kalgebra ${KDE4_KDEUI_LIBS} ${QT_QTOPENGL_LIBRARY})

install(TARGETS kalgebra DESTINATION bin)

