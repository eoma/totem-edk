BEGIN TRANSACTION;

CREATE TABLE GameObjects
(
	GameObjectId INT NOT NULL,
	GameObjectContainerId INT,
	LoadByDefault INT DEFAULT(1),
	Description TEXT,

	PRIMARY KEY(GameObjectId)
);

CREATE TABLE GameObjectComponents 
(
	GameObjectId INT NOT NULL,
	Type TEXT NOT NULL,
	Name TEXT,

	PRIMARY KEY(GameObjectId, Name)
);

CREATE TABLE GameObjectProperties 
(
	GameObjectId INT NOT NULL,
	ComponentType TEXT NULL,
	ComponentName TEXT NULL,
	Type INT NOT NULL,
	Name TEXT NOT NULL,
	Value TEXT NOT NULL,

	PRIMARY KEY(GameObjectId, Name)
);

COMMIT;
