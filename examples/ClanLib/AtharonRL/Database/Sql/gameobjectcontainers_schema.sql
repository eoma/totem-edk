BEGIN TRANSACTION;

CREATE TABLE GameObjectContainers
(
	GameObjectContainerId INT NOT NULL,
	Description TEXT NOT NULL,

	PRIMARY KEY(GameObjectContainerId)
);

COMMIT;
