BEGIN TRANSACTION;

CREATE TABLE ZoneInstances 
(
	GameObjectContainerId INT NOT NULL,
	GenerationSeed INT NOT NULL
);

COMMIT;
