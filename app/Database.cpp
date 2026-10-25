#include <iostream>
#include "Database.h"

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>

Database::Database()
{
    sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();

    myConnection = driver->connect("tcp://127.0.0.1:3306", "root", "Gael77523912Root*");
  
    // San Francisco spelled wrong for schema (franciso)
    myConnection->setSchema("sanfrancisopubliclibrary");
}

void Database::viewPatronTypes()
{
    sql::Statement* command;
    sql::ResultSet* row;

    command = myConnection->createStatement();

    row = command->executeQuery("SELECT * FROM patron_type");

    while (row->next())
    {
		cout << row->getInt("patron_type_code") << " - " << row->getString("patron_type_definition") << endl;
	}   
}
