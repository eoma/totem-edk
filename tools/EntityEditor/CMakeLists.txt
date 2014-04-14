###
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
# 
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
#
# Note: Some of the libraries Totem EDK may link to may have additional
# requirements or restrictions.
#
###
# CMake system for Totem EDK was contributed by Jacob 'jacmoe' Moen
#
###
#---------------------------------------------------------------
cmake_minimum_required(VERSION 2.8.3)
#---------------------------------------------------------------
project(ToolEntityEditor)
#---------------------------------------------------------------
set(UIC_UI
	./ui/mainWindow.ui
)
SOURCE_GROUP("Ui Files" FILES ${UIC_UI})
#---------------------------------------------------------------
set(HDRS
	./MainWindow.h
)
set(SRCS
  ./main.cpp
  ./MainWindow.cpp
)
#---------------------------------------------------------------
find_package(OpenGL REQUIRED)
find_package(Glew REQUIRED)
find_package(Qt4 4.7.3 REQUIRED QtCore QtGui QtOpenGL)
#---------------------------------------------------------------
include_directories(${OPENGL_INCLUDE_PATH})
include_directories(${GLEW_INCLUDE_PATH})
include(${QT_USE_FILE})
include(./cmake/Config.cmake)
CMP_QT_LIBRARIES_INSTALL_RULES("QtCore;QtGui;QtOpenGL" ${ToolEntityEditor_BINARY_DIR})
#---------------------------------------------------------------
QT4_WRAP_CPP(MOCS ${GV_HDRS} ${CLIENT_HDRS} ${UI_HDRS})
SOURCE_GROUP("Moc Files" FILES ${MOCS})
#---------------------------------------------------------------
QT4_WRAP_UI(UICS ${UIC_UI})
SOURCE_GROUP("Uic Files" FILES ${UICS})
#---------------------------------------------------------------
add_executable(ToolEntityEditor 
	${HDRS} ${SRCS}
	${MOCS} ${UICS}
)
#---------------------------------------------------------------
set_target_properties(ToolEntityEditor PROPERTIES DEBUG_POSTFIX _d)
set_target_properties(ToolEntityEditor PROPERTIES FOLDER "Tools")
#---------------------------------------------------------------
target_link_libraries(ToolEntityEditor
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARY}
  ${QT_LIBRARIES}
  TotemEDK
)
