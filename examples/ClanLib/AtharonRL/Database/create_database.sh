echo MUHAHAHAHAHA, THIS JUST ERASED YOUR PRECIOUS DATABASE, SUCKER!
echo --------------------------------------------------------------
rm game.db
echo - users_schema
cat Sql/users_schema.sql | sqlite3 game.db
echo - users_data
cat Sql/users_data.sql | sqlite3 game.db
echo - gameobjects_schema
cat Sql/gameobjects_schema.sql | sqlite3 game.db
echo - gameobjects_schema
cat Sql/gameobjects_data.sql | sqlite3 game.db
echo - gameobjectcontainers_schema
cat Sql/gameobjectcontainers_schema.sql | sqlite3 game.db
echo - gameobjectcontainers_data
cat Sql/gameobjectcontainers_data.sql | sqlite3 game.db
echo - characters_schema
cat Sql/characters_schema.sql | sqlite3 game.db
echo - characters_data
cat Sql/characters_data.sql | sqlite3 game.db
echo - email_schema
cat Sql/email_schema.sql | sqlite3 game.db
echo - zoneinstances_schema
cat Sql/zoneinstances_schema.sql | sqlite3 game.db
echo - zoneinstances_data
cat Sql/zoneinstances_data.sql | sqlite3 game.db
echo Database created
