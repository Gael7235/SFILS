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
    sql::Statement* command = myConnection->createStatement();
    sql::ResultSet* row = row = command->executeQuery("SELECT * FROM patron_type");

    while (row->next())
    {
		cout << row->getInt("patron_type_code") << " - " << row->getString("patron_type_definition") << endl;
	}   
}

void Database::viewLibraries()
{
	sql::Statement* command = myConnection->createStatement();
    sql::ResultSet* row = command->executeQuery("SELECT * FROM home_library");

	while (row->next())
    {
		cout << row->getString("home_library_code") << " - " << row->getString("home_library_definition") << endl;
	}   
}

void Database::viewNotificationMediums()
{
	sql::Statement* command = myConnection->createStatement();
    sql::ResultSet* row = command->executeQuery("SELECT * FROM notification_medium");

	while (row->next())
    {
		cout << row->getString("notification_medium_code") << " - " << row->getString("notification_code_definition") << endl;
	}  
}

void Database::searchPatronType()
{
	string input;

    cin.ignore();
    getline(cin, input);

    sql::PreparedStatement* prepCommand = myConnection->prepareStatement("SELECT p.patron_type_definition, u.checkout_total 
															 		  "FROM sfpl_usage u "
															 		  "JOIN patron_types p "
															 		  "ON u.patron_type_code = p.patron_type_code"
															 		  "WHERE p.patron_type_definition = ?");

    prepCommand->setString(1, input);

    sql::ResultSet* row = prepCommand->executeQuery();

    while(row->next())
    {
        cout << row->getString("patron_type_definition") << " " << row->getInt("checkout_total") << endl;
    }
}

void Database::searchLibrary()
{
	string input;

    cin.ignore();
    getline(cin, input);

    sql::PreparedStatement* prepCommand = myConnection->prepareStatement("SELECT l.home_library_definition, u.checkout_total 
															 		  "FROM sfpl_usage u "
															 		  "JOIN home_library l "
															 		  "ON u.home_library_code = p.home_library_code"
															 		  "WHERE p.home_library_definition = ?");

    prepCommand->setString(1, input);

    sql::ResultSet* row = prepCommand->executeQuery();

    while(row->next())
    {
        cout << row->getString("home_library_definition") << " " << row->getInt("checkout_total") << endl;
    }
}

void Database::totalCheckoutsByPatronType()
{
	sql::Statement* command = myConnection->createStatement();
    sql::ResultSet* row = command->executeQuery("SELECT p.patron_type_definition, "
        										"SUM(u.checkout_total) total "
        										"FROM sfpl_usage u "
        										"JOIN patron_type p "
        										"ON p.patron_type_code = u.patron_type_code "
        										"GROUP BY p.patron_type_definition "
        										"ORDER BY total DESC");
	while(row->next())
    {
        cout << row->getString("patron_type_definition") << " : " << row->getInt("total") << endl;
    }
}

void Database::totalCheckoutsByLibrary()
{
	sql::Statement* command = myConnection->createStatement();
    sql::ResultSet* row = command->executeQuery("SELECT p.home_library_definition 	, "
        										"SUM(u.checkout_total) total "
        										"FROM sfpl_usage u "
        										"JOIN home_library p "
        										"ON p.home_library_code = u.home_library_code "
        										"GROUP BY p.home_library_definition 	 "
        										"ORDER BY total DESC");
	while(row->next())
    {
        cout << row->getString("home_library_definition 	") << " : " << row->getInt("total") << endl;
    }
}

void Database::addUsageRecord()
{
	int input_patronCode;
    string input_libraryCode;
    int input_checkouts;

    cout << "Patron Code: ";
    cin >> input_patronCode;

    cout << "Library Code: ";
    cin >> input_libraryCode;

    cout << "Checkouts: ";
    cin >> input_checkouts;

    sql::PreparedStatement* prepCommand = myConnection->prepareStatement("INSERT INTO library_usage "
        													 "(patron_type_code, home_library_code, checkout_total)VALUES(?,?,?)");

    prepCommand->setInt(1, input_patronCode);
    prepCommand->setString(2, input_libraryCode);
    prepCommand->setInt(3, input_checkouts);

    prepCommand->execute();
}

void Database::updateUsageRecord()
{
	int input_id;
    int input_checkouts;

    cout << "Usage ID: ";
    cin >> input_id;

    cout << "New Checkout Total: ";
    cin >> input_checkouts;

    sql::PreparedStatement* prepCommand = myConnection->prepareStatement("UPDATE library_usage "
        												  "SET checkout_total=? "
       													  "WHERE usage_id=?");

    prepCommand->setInt(1, input_checkouts);
    prepCommand->setInt(2, input_id);

    prepCommand->execute();
}

void Database::deleteUsageRecord()
{
	int input_id;

    cout << "Usage ID: ";
    cin >> input_id;

    sql::PreparedStatement* prepCommand = myConnection->prepareStatement("DELETE FROM library_usage "
        														"WHERE usage_id=?");

    prepCommand->setInt(1, input_id);

    prepCommand->execute();
}
