CXXFLAGS = -std=c++11 -g -I . -I Include -I Sources/Server -I Sources -I Include/LibTCOD `pkg-config --cflags clanDisplay-3.0 clanCore-3.0 clanApp-3.0 clanSqlite-3.0 clanNetwork-3.0`
libs = `pkg-config --libs clanDisplay-3.0 clanCore-3.0 clanApp-3.0 clanSqlite-3.0 clanNetwork-3.0` -lpthread -ldl

Server = \
	Sources/Server/main.o \
	Sources/Server/precomp.o \
	Sources/Server/service.o \
	Sources/Server/server.o \
	Sources/Server/server_user.o \
	Sources/Server/Database/database_characters.o \
	Sources/Server/Database/database_email.o \
	Sources/Server/Database/database_gameobject_containers.o \
	Sources/Server/Database/database_gameobjects.o \
	Sources/Server/Database/database_users.o \
	Sources/Server/Database/database_zone_instances.o \
	Sources/Server/GameWorld/character.o \
	Sources/Server/GameWorld/character_manager.o \
	Sources/Server/GameWorld/component_factory.o \
	Sources/Server/GameWorld/gameobject_container.o \
	Sources/Server/GameWorld/player.o \
	Sources/Server/GameWorld/player_manager.o \
	Sources/Server/GameWorld/replicated_component.o \
	Sources/Server/GameWorld/server_gameobject.o \
	Sources/Server/GameWorld/Components/network_receiver.o \
	Sources/Server/GameWorld/Components/server_transform.o \
	Sources/Server/GameWorld/Components/server_ascii_visual.o \
	Sources/Server/GameWorld/Netevents/netevents_character.o \
	Sources/Server/GameWorld/Netevents/netevents_player.o \
	Sources/Server/GameWorld/Zone/zone.o \
	Sources/Server/GameWorld/Zone/zone_architect.o \
	Sources/Server/GameWorld/Zone/zone_manager.o \
	Sources/Server/GameWorld/Zone/zone_map.o \
	Sources/Server/GameWorld/Zone/zone_map_section.o \
	Sources/Server/GameWorld/Zone/zone_player_manager.o \
	Sources/Server/GameWorld/Zone/zone_vicinity_map.o \
	Sources/Server/GameWorld/Zone/zone_vicinity_objects.o \
	Sources/Server/GameWorld/Zones/dungeon.o \
	Sources/Server/GameWorld/Zones/dungeon_level.o \
	Sources/Server/GameWorld/Zones/town.o \
	Sources/Engine/Common/GameWorld/gameobject.o \
	Sources/Engine/Common/GameWorld/property_serializer.o \
	Sources/Engine/Common/GameWorld/component_serializer.o \
	Sources/Engine/Common/GameWorld/tick_timer.o \
	Sources/Engine/Common/GameWorld/Components/ascii_visual.o \
	Sources/Engine/Common/GameWorld/Components/transform.o

all: server

clean:
	rm server `find . -name *.o`

COMPILE.cxx = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
OUTPUT_OPTION = -o $@

%.o: %.cxx
	$(COMPILE.cxx) $< $(OUTPUT_OPTION)

server: $(Server) 
	g++ -o server $(Server) $(libs)
	echo All done
