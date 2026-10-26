-- Create your username along with your user password
CREATE USER 'guest_user_sfpl'@'localhost'
IDENTIFIED BY 'mypassword123';

-- Make sure this user has all privileges
GRANT ALL PRIVILEGES
ON sfpl_db.* -- <-Name of the schema
TO 'guest_user_sfpl'@'localhost';

-- Default mysql_native_password won't work so we need to convert to caching_sha2_password
ALTER USER 'guest_user_sfpl'@'localhost'
IDENTIFIED WITH caching_sha2_password
BY 'mypassword123';

-- Shows all users and whether it worked or not
SELECT user, host, plugin
FROM mysql.user;

-- Refreshes privileges
FLUSH PRIVILEGES;

-- Make sure you remember the exact "username", "hostname" (could be 127.0.0.1:3306 but in this case we set it to localhost), and "password" to insert into the app interface code.
-- In this case you don't need to worry about the code part as it is already matched up, you just need to copy and paste and run this inside of MySQL.
-- IMPORTANT: Proceed in creating a new DB Connection (separate from the root connection) in MySQL using these credentials. (if you are using localhost in this case make sure to change the hostname to "localhost")
