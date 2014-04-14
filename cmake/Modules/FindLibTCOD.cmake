FIND_PATH(TCOD_DIR include/libtcod.hpp
      ${CMAKE_SOURCE_DIR}/depends/Standard/Examples/Roguelike/
    )
	
FIND_PATH(TCOD_INCLUDE libtcod.hpp
      ${CMAKE_SOURCE_DIR}/depends/Standard/Examples/Roguelike/include/
    )

FIND_LIBRARY(TCOD_LIBRARY
  NAMES libtcod-VS
  PATHS ${CMAKE_SOURCE_DIR}/depends/Standard/Examples/Roguelike/
    )
	
FIND_LIBRARY(TCOD_GUI_LIBRARY
  NAMES libtcod-gui-VS
  PATHS ${CMAKE_SOURCE_DIR}/depends/Standard/Examples/Roguelike/
    )
