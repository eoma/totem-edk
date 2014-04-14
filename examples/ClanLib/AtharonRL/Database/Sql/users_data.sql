BEGIN TRANSACTION;

INSERT INTO Users (UserId, UserName, Password, RealName) VALUES (1, 'sphair', 'p', 'Kenneth Gangstoe');
INSERT INTO Users (UserId, UserName, Password, RealName) VALUES (2, 'ptrefall', 'p', 'Pål Trefall');
INSERT INTO Users (UserId, UserName, Password, RealName) VALUES (3, 'billowe', 'p', 'Bill Lowe');

COMMIT;
