FIND_PATH(CL_DIR core.h
      ${CMAKE_SOURCE_DIR}/Include/ClanLib/
	  "D:/Development/C++ Packets/ClanLib3.0/Include/ClanLib/"
	  "D:/Development/3rdParty/ClanLib/include/ClanLib/"
	  "H:/SourceCompile/includeClanLib30/ClanLib"
	  "F:/source_private/includeClanLib30/ClanLib"
    )
	
FIND_PATH(CL_INCLUDE ClanLib/core.h
	  ${CMAKE_SOURCE_DIR}/Include/
	  "D:/Development/C++ Packets/ClanLib3.0/Include/"
	  "D:/Development/3rdParty/ClanLib/include/"
	  "H:/SourceCompile/includeClanLib30"
	  "F:/source_private/includeClanLib30"
	)
	
SET(LIB_PATHS 
	"${CMAKE_SOURCE_DIR}/Lib/Win32/" 
	"D:/Development/C++ Packets/ClanLib3.0/Lib/Win32/"
	"D:/Development/3rdParty/ClanLib/lib/Win32/"
	"H:/SourceCompile/libClanLib30/Win32"
	"F:/source_private/libClanLib30/Win32"
)

FIND_LIBRARY(CL_APP_LIBRARY_DEBUG
  NAMES clanApp-static-mt-debug
  PATHS ${LIB_PATHS}
    )

FIND_LIBRARY(CL_APP_LIBRARY_RELEASE
  NAMES clanApp-static-mt
  PATHS ${LIB_PATHS}
    )

FIND_LIBRARY(CL_CORE_LIBRARY_DEBUG
  NAMES clanCore-static-mt-debug
  PATHS ${LIB_PATHS}
    )

FIND_LIBRARY(CL_CORE_LIBRARY_RELEASE
  NAMES clanCore-static-mt
  PATHS ${LIB_PATHS}
    )
	
FIND_LIBRARY(CL_DB_LIBRARY_DEBUG
  NAMES clanDatabase-static-mt-debug
  PATHS ${LIB_PATHS}
    )

FIND_LIBRARY(CL_DB_LIBRARY_RELEASE
  NAMES clanDatabase-static-mt
  PATHS ${LIB_PATHS}
    )
	
FIND_LIBRARY(CL_DISPLAY_LIBRARY_DEBUG
  NAMES clanDisplay-static-mt-debug
  PATHS ${LIB_PATHS}
    )

FIND_LIBRARY(CL_DISPLAY_LIBRARY_RELEASE
  NAMES clanDisplay-static-mt
  PATHS ${LIB_PATHS}
    )
	
FIND_LIBRARY(CL_GL_LIBRARY_DEBUG
  NAMES clanGL-static-mt-debug
  PATHS ${LIB_PATHS}
    )

FIND_LIBRARY(CL_GL_LIBRARY_RELEASE
  NAMES clanGL-static-mt
  PATHS ${LIB_PATHS}
    )

FIND_LIBRARY(CL_NET_LIBRARY_DEBUG
  NAMES clanNetwork-static-mt-debug
  PATHS ${LIB_PATHS}
    )

FIND_LIBRARY(CL_NET_LIBRARY_RELEASE
  NAMES clanNetwork-static-mt
  PATHS ${LIB_PATHS}
    )
	
FIND_LIBRARY(CL_SOUND_LIBRARY_DEBUG
  NAMES clanSound-static-mt-debug
  PATHS ${LIB_PATHS}
    )

FIND_LIBRARY(CL_SOUND_LIBRARY_RELEASE
  NAMES clanSound-static-mt
  PATHS ${LIB_PATHS}
    )
	
FIND_LIBRARY(CL_SQ_LIBRARY_DEBUG
  NAMES clanSqlite-static-mt-debug
  PATHS ${LIB_PATHS}
    )

FIND_LIBRARY(CL_SQ_LIBRARY_RELEASE
  NAMES clanSqlite-static-mt
  PATHS ${LIB_PATHS}
    )
	
FIND_LIBRARY(CL_VORBIS_LIBRARY_DEBUG
  NAMES clanVorbis-static-mt-debug
  PATHS ${LIB_PATHS}
    )

FIND_LIBRARY(CL_VORBIS_LIBRARY_RELEASE
  NAMES clanVorbis-static-mt
  PATHS ${LIB_PATHS}
    )
	
FIND_LIBRARY(CL_GUI_LIBRARY_DEBUG
  NAMES clanGUI-static-mt-debug
  PATHS ${LIB_PATHS}
    )

FIND_LIBRARY(CL_GUI_LIBRARY_RELEASE
  NAMES clanGUI-static-mt
  PATHS ${LIB_PATHS}
    )
	
FIND_LIBRARY(CL_CSS_LIBRARY_DEBUG
  NAMES clanCSSLayout-static-mt-debug
  PATHS ${LIB_PATHS}
    )

FIND_LIBRARY(CL_CSS_LIBRARY_RELEASE
  NAMES clanCSSLayout-static-mt
  PATHS ${LIB_PATHS}
    )