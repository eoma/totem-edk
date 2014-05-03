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

project(TestNewApproach)

set(TOTEM_HDRS
  ../../../editions/ClanLib/include/Totem/IComponent.h
  ../../../editions/ClanLib/include/Totem/Component.h
  ../../../editions/ClanLib/include/Totem/ComponentContainer.h
  ../../../editions/ClanLib/include/Totem/IProperty.h
  ../../../editions/ClanLib/include/Totem/Property.h
  ../../../editions/ClanLib/include/Totem/PropertyContainer.h
  ../../../editions/ClanLib/include/Totem/IPropertyList.h
  ../../../editions/ClanLib/include/Totem/PropertyList.h
  ../../../editions/ClanLib/include/Totem/PropertyListContainer.h
  ../../../editions/ClanLib/include/Totem/HashedString.h
)
SOURCE_GROUP("Header Files\\Totem" FILES ${TOTEM_HDRS})

set(TOTEM_EXT_HDRS
  ../../../editions/ClanLib/include/Totem/Extensions/EventSystem.h
)
SOURCE_GROUP("Header Files\\Totem\\Extensions" FILES ${TOTEM_EXT_HDRS})

set(HDRS
  ${TOTEM_HDRS}
  ${TOTEM_EXT_HDRS}
  ../Common/Entity.h
  ./TestSystem.h
  ./TestComponent.h
)

set(SRCS
  ./main.cpp
  ./TestComponent.cpp
)
 
add_executable(TestNewApproach ${HDRS} ${SRCS})

set_target_properties(TestNewApproach PROPERTIES DEBUG_POSTFIX _d)
set_target_properties(TestNewApproach PROPERTIES FOLDER "Tests")
