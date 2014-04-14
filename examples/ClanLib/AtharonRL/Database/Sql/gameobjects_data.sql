BEGIN TRANSACTION;


-- ORC
INSERT INTO GameObjects (GameObjectId, GameObjectContainerId, LoadByDefault, Description) VALUES (11, 1, 1, 'Orc');
INSERT INTO GameObjectComponents (GameObjectId, Type) VALUES (11, "AsciiVisual");
INSERT INTO GameObjectComponents (GameObjectId, Type) VALUES (11, "Transform");
INSERT INTO GameObjectProperties (GameObjectId, Type, Name, Value) VALUES (11, 2, "Visual", "1");
INSERT INTO GameObjectProperties (GameObjectId, Type, Name, Value) VALUES (11, 101, "Position", "10 10");

-- TRIGGER
--INSERT INTO GameObjects (GameObjectId, GameObjectContainerId, LoadByDefault, Description) VALUES (12, 1, 1, 'Trigger');
--INSERT INTO GameObjectComponents (GameObjectId, Type) VALUES (12, "Collider");
--INSERT INTO GameObjectProperties (GameObjectId, Type, Name, Value) VALUES (12, 101, "Position", "15 2");
--INSERT INTO GameObjectComponents (GameObjectId, Type) VALUES (12, "TriggerDisplayText");
--INSERT INTO GameObjectProperties (GameObjectId, Type, Name, Value) VALUES (12, 100, "DisplayTextOnEnter", "You entered the trigger");
--INSERT INTO GameObjectProperties (GameObjectId, Type, Name, Value) VALUES (12, 100, "DisplayTextOnExit", "You left the trigger");

-- PLAYER ECO
INSERT INTO GameObjects (GameObjectId, GameObjectContainerId, LoadByDefault, Description) VALUES (1, 1, 0, 'Eco');
INSERT INTO GameObjectComponents (GameObjectId, Type) VALUES (1, "AsciiVisual");
INSERT INTO GameObjectComponents (GameObjectId, Type) VALUES (1, "Transform");
INSERT INTO GameObjectProperties (GameObjectId, Type, Name, Value) VALUES (1, 2, 	"Visual", "0");
INSERT INTO GameObjectProperties (GameObjectId, Type, Name, Value) VALUES (1, 101, 	"Position", "12 15");
INSERT INTO GameObjectProperties (GameObjectId, Type, Name, Value) VALUES (1, 102, 	"ColorForeground", "1 0 0 1");

-- PLAYER Woody
INSERT INTO GameObjects (GameObjectId, GameObjectContainerId, LoadByDefault, Description) VALUES (2, 1, 0, 'Woody');
INSERT INTO GameObjectComponents (GameObjectId, Type) VALUES (2, "AsciiVisual");
INSERT INTO GameObjectComponents (GameObjectId, Type) VALUES (2, "Transform");
INSERT INTO GameObjectProperties (GameObjectId, Type, Name, Value) VALUES (2, 2, 	"Visual", "0");
INSERT INTO GameObjectProperties (GameObjectId, Type, Name, Value) VALUES (2, 101, 	"Position", "12 12");
INSERT INTO GameObjectProperties (GameObjectId, Type, Name, Value) VALUES (2, 102, 	"ColorForeground", "0 1 0 1");

-- PLAYER Billeh
INSERT INTO GameObjects (GameObjectId, GameObjectContainerId, LoadByDefault, Description) VALUES (3, 1, 0, 'Billeh');
INSERT INTO GameObjectComponents (GameObjectId, Type) VALUES (3, "AsciiVisual");
INSERT INTO GameObjectComponents (GameObjectId, Type) VALUES (3, "Transform");
INSERT INTO GameObjectProperties (GameObjectId, Type, Name, Value) VALUES (3, 2, 	"Visual", "0");
INSERT INTO GameObjectProperties (GameObjectId, Type, Name, Value) VALUES (3, 101, 	"Position", "12 12");
INSERT INTO GameObjectProperties (GameObjectId, Type, Name, Value) VALUES (3, 102, 	"ColorForeground", "0 0 1 1");

-- TYPE_UNSIGNED_INT = 1,
-- TYPE_INT = 2,
-- TYPE_FLOAT = 3,
-- TYPE_BOOL = 4,
-- TYPE_DOUBLE = 5,
-- TYPE_CHAR = 6,
-- TYPE_CL_STRING = 100,
-- TYPE_CL_VEC2I = 101,				// x y z
-- TYPE_CL_COLORF = 102,			// r g b a

COMMIT;
