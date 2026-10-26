1. Create the schema/DB in a root MySQL connection, name the DB "sfpl_db" to match with my code.
2. Once you've enabled local_infile you need to reload the connection so close it and then reconnect. Then execute the scripts in the scripts repository in this root connection (you need to first follow the instructions to enable local_infile to import the data).
3. Once the database is created, follow the instructions in the app repository.

All screenshots for visual instructions are located in the docs repository.
