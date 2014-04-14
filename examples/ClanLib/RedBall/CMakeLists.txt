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

project(RedBall)

MACRO(ADD_MSVC_PRECOMPILED_HEADER PrecompiledHeader PrecompiledSource SourcesVar)
  IF(MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${PrecompiledHeader} NAME_WE)
    SET(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
    SET(Sources ${${SourcesVar}})

    SET_SOURCE_FILES_PROPERTIES(${PrecompiledSource}
                                PROPERTIES COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_OUTPUTS "${PrecompiledBinary}")
    SET_SOURCE_FILES_PROPERTIES(${Sources}
                                PROPERTIES COMPILE_FLAGS "/Yu\"${PrecompiledBinary}\" /FI\"${PrecompiledBinary}\" /Fp\"${PrecompiledBinary}\""
                                           OBJECT_DEPENDS "${PrecompiledBinary}")  
    # Add precompiled header to SourcesVar
    LIST(APPEND ${SourcesVar} ${PrecompiledSource})
  ENDIF(MSVC)
ENDMACRO(ADD_MSVC_PRECOMPILED_HEADER)

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

set(TOTEM_ADDONS_HDRS
  ../../../editions/ClanLib/include/Totem/Addons/EventSystem.h
)
SOURCE_GROUP("Header Files\\Totem\\Addons" FILES ${TOTEM_ADDONS_HDRS})

ADD_MSVC_PRECOMPILED_HEADER("precomp.h" "precomp.cpp" PRECOMP)

set(HDRS
  ${TOTEM_HDRS}
  ${TOTEM_ADDONS_HDRS}
  entity.h
  redballapp.h
  visual_manager.h
  component_visual.h
  component_colorshift.h
  component_bounce.h
)

set(SRCS
  ${PRECOMP}
  entity.cpp
  redballapp.cpp
  visual_manager.cpp
  component_visual.cpp
  component_colorshift.cpp
  component_bounce.cpp
)
 
add_executable(RedBall ${PRECOMP} ${HDRS} ${SRCS})

set_target_properties(RedBall PROPERTIES DEBUG_POSTFIX _d)
set_target_properties(RedBall PROPERTIES FOLDER "Examples")
