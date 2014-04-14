#pragma once

// Client to Server (CTS) Events

#define CTS_PING						"Ping"					// (int time)

#define CTS_LOGIN						"Login"					// (string playerName, string password)

#define CTS_CHARACTER_GETLIST			"CharacterGetList"		// ()
#define CTS_CHARACTER_LOGIN				"CharacterLogin"		// (int characterId)
#define CTS_CHARACTER_LOGOUT			"CharacterLogout"		// ()
#define CTS_CHARACTER_CREATE			"CharacterCreate"		// (string name)

#define CTS_OBJECT_EVENT				"ObjectEvent"			// (int gameObjectId, string eventName, [marshalledEvent]...)

#define CTS_PLAYER_MOVE					"PlayerMove"			// (int delta_x, int delta_y)

// Server to Client (STC) Events

#define STC_PONG						"Pong"					// (int time)
#define STC_MOTD						"Motd"					// (string html)

#define STC_LOGIN_SUCCESS				"LoginSuccess"			// ()
#define STC_LOGIN_FAIL					"LoginFail"				// (string reason)
#define STC_LOGOUT						"LoggedOut"				// (string reason)

#define STC_CHARACTER_LIST				"CharacterList"			// (unsigned int characterCount, [int characterId, string characterName]...)
#define STC_CHARACTER_LOGIN_SUCCESS		"CharacterLoginSuccess"	// ()
#define STC_CHARACTER_LOGIN_FAIL		"CharacterLoginFail"	// (string reason)
#define STC_CHARACTER_LOGOUT			"CharacterLogout"		// (string reason)
#define STC_CHARACTER_CREATE_SUCCESS	"CharacterCreateSuccess"// (int characterId)
#define STC_CHARACTER_CREATE_FAIL		"CharacterCreateFail"	// (string reason)

#define STC_TEXT_MESSAGE				"TextMessage"			// (string message)

#define STC_CHAT_MESSAGE				"ChatMessage"			// (string channel, string sender, string message)

#define STC_ZONE						"Zone"					// (string zoneName)
#define STC_OBJECT_CREATE				"ObjectCreate"			// (int gameObjectId, string gameObjectName, unsigned int componentCount, [string componentType, string componentName, unsigned int propertyCount, [int propertyTypeId, string propertyName, string propertyValue]])
#define STC_OBJECT_UPDATE				"ObjectUpdate"			// (int gameObjectId, unsigned int propertyCount, [int propertyTypeId, string propertyName, string propertyValue]])
#define STC_OBJECT_DESTROY				"ObjectDestroy"			// (int gameObjectId)
#define STC_OBJECT_PLAYER_OWN			"ObjectPlayerOwn"		// (int gameObjectId)
#define STC_OBJECT_EVENT				"ObjectEvent"			// (int gameObjectId, string eventName, [marshalledEvent]...)

#define STC_MAP_CREATE					"MapCreate"				// ()
#define STC_MAP_UPDATE					"MapUpdate"				// ()
#define STC_MAP_DESTROY					"MapDestroy"			// ()
#define STC_MAP_EVENT					"MapEvent"				// ()
