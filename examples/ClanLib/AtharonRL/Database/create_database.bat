@echo off
rem echo WARNING - THIS WILL DELETE EXISTING DATABASE
echo MUHAHAHAHAHA, THIS JUST ERASED YOUR PRECIOUS DATABASE, SUCKER!
echo --------------------------------------------------------------
rem pause

del game.db

echo - users_schema
type Sql\users_schema.sql | sqlite3.exe game.db
echo - users_data
type Sql\users_data.sql | sqlite3.exe game.db

echo - gameobjects_schema
type Sql\gameobjects_schema.sql | sqlite3.exe game.db
echo - gameobjects_schema
type Sql\gameobjects_data.sql | sqlite3.exe game.db

echo - gameobjectcontainers_schema
type Sql\gameobjectcontainers_schema.sql | sqlite3.exe game.db
echo - gameobjectcontainers_data
type Sql\gameobjectcontainers_data.sql | sqlite3.exe game.db

echo - characters_schema
type Sql\characters_schema.sql | sqlite3.exe game.db
echo - characters_data
type Sql\characters_data.sql | sqlite3.exe game.db

echo - email_schema
type Sql\email_schema.sql | sqlite3.exe game.db

echo - zoneinstances_schema
type Sql\zoneinstances_schema.sql | sqlite3.exe game.db
echo - zoneinstances_data
type Sql\zoneinstances_data.sql | sqlite3.exe game.db

echo Database created

pause
