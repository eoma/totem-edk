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

cmake_minimum_required(VERSION 2.8.3)

set(NAME TestCoreFunctionality)
project(${NAME})

set(TOTEM_HDRS
  ${CMAKE_SOURCE_DIR}/include/Totem/IComponent.h
  ${CMAKE_SOURCE_DIR}/include/Totem/Component.h
  ${CMAKE_SOURCE_DIR}/include/Totem/ComponentContainer.h
  ${CMAKE_SOURCE_DIR}/include/Totem/IProperty.h
  ${CMAKE_SOURCE_DIR}/include/Totem/Property.h
  ${CMAKE_SOURCE_DIR}/include/Totem/PropertyContainer.h
  ${CMAKE_SOURCE_DIR}/include/Totem/IPropertyList.h
  ${CMAKE_SOURCE_DIR}/include/Totem/PropertyList.h
  ${CMAKE_SOURCE_DIR}/include/Totem/PropertyListContainer.h
  ${CMAKE_SOURCE_DIR}/include/Totem/HashedString.h
)
SOURCE_GROUP("Header Files\\Totem" FILES ${TOTEM_HDRS})

set(TOTEM_EXT_HDRS
  ${CMAKE_SOURCE_DIR}/include/Totem/Extensions/EventSystem.h
  ${CMAKE_SOURCE_DIR}/include/Totem/Extensions/DelegateSystem.h
)
SOURCE_GROUP("Header Files\\Totem\\Extensions" FILES ${TOTEM_EXT_HDRS})

set(HDRS
  ${TOTEM_HDRS}
  ${TOTEM_EXT_HDRS}
)

set(SRCS
  main.cpp
)
 
add_executable(${NAME} ${HDRS} ${SRCS})

set_target_properties(${NAME} PROPERTIES DEBUG_POSTFIX _d)
set_target_properties(${NAME} PROPERTIES FOLDER "Tests")

include_directories(
  "${CMAKE_SOURCE_DIR}/depends/Standard/"
)
