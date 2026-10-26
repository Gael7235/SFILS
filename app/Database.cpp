#include <iostream>
#include "Database.h"
#include <string>
#include <iomanip>

using namespace std;

Database::Database()
{
    try
    {
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();

        myConnection = driver->connect("tcp://localhost:3306", "sfils_user", "Library123!");

        myConnection->setSchema("SFPL_DB");
    }
    catch (const sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << endl;
        std::cerr << "Error Code: " << e.getErrorCode() << endl;
        std::cerr << "SQL State: " << e.getSQLState() << endl;
    }
}

void Database::viewPatronTypes()
{
    sql::Statement* command = myConnection->createStatement();
    sql::ResultSet* row = row = command->executeQuery("SELECT * FROM patron_type");

    while (row->next())
    {
        cout << row->getInt("patron_type_code") << "  -  " << row->getString("patron_type_definition") << endl;
    }
}

void Database::viewLibraries()
{
    sql::Statement* command = myConnection->createStatement();
    sql::ResultSet* row = command->executeQuery("SELECT * FROM home_library");

    while (row->next())
    {
        cout << row->getString("home_library_code") << "  -  " << row->getString("home_library_definition") << endl;
    }
}

void Database::viewNotificationMediums()
{
    sql::Statement* command = myConnection->createStatement();
    sql::ResultSet* row = command->executeQuery("SELECT * FROM notification_medium");

    while (row->next())
    {
        cout << row->getString("notification_medium_code") << "  -  " << row->getString("notification_code_definition") << endl;
    }
}

void Database::searchPatronType()
{
    string input;

    string ans;
    cout << "Show options? (y/n) ";
    cin >> ans;
    if (ans == "y") {cout << endl; viewPatronTypes(); cout << endl;}

    cout << "Enter patron type: ";
    cin.ignore();
    getline(cin, input);

    sql::PreparedStatement* prepCommand = myConnection->prepareStatement("SELECT p.patron_type_definition, u.checkout_total "
                                                                         "FROM sfpl_usage u "
                                                                         "JOIN patron_type p "
                                                                         "ON u.patron_type_code = p.patron_type_code "
                                                                         "WHERE p.patron_type_definition = ? ");

    prepCommand->setString(1, input);

    sql::ResultSet* row = prepCommand->executeQuery();

    while (row->next())
    {
        cout << row->getString("patron_type_definition") << " " << row->getInt("checkout_total") << endl;
    }
}

void Database::searchLibrary()
{
    string input;

    string ans;
    cout << "Show options? (y/n) ";
    cin >> ans;
    if (ans == "y") { cout << endl; viewPatronTypes(); cout << endl; }

    cout << "Enter Home Library: ";
    cin.ignore();
    getline(cin, input);

    sql::PreparedStatement* prepCommand = myConnection->prepareStatement("SELECT l.home_library_definition, u.checkout_total "
        "FROM sfpl_usage u "
        "JOIN home_library l "
        "ON u.home_library_code = p.home_library_code "
        "WHERE p.home_library_definition = ?");

    prepCommand->setString(1, input);

    sql::ResultSet* row = prepCommand->executeQuery();

    while (row->next())
    {
        cout << row->getString("home_library_definition") << " " << row->getInt("checkout_total") << endl;
    }
}

void Database::searchByID()
{
    string input;

    cout << "Enter ID: ";
    cin.ignore();
    getline(cin, input);

    int int_input = stoi(input);

    sql::PreparedStatement* prepCommand = myConnection->prepareStatement("SELECT u.id, p.patron_type_definition, u.patron_age_range, u.checkout_total, u.renewal_total, h.home_library_definition, n.notification_code_definition, u.provided_email_address, u.circulation_active_year, u.circulation_active_month, u.within_sf_county, u.year_patron_registered "
                                                                         "FROM sfpl_usage u "
                                                                         "JOIN patron_type p "
                                                                         "ON u.patron_type_code = p.patron_type_code "
                                                                         "JOIN home_library h "
                                                                         "ON u.home_library_code = h.home_library_code "
                                                                         "JOIN notification_medium n "
                                                                         "ON u.notification_medium_code = n.notification_medium_code "
                                                                         "WHERE u.id = ?");

    prepCommand->setInt(1, int_input);

    sql::ResultSet* row = prepCommand->executeQuery();

    cout << "_______________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << setw(20) << left << "|ID" << setw(20) << "|Patron Type" << setw(20) << "|Age Range" << setw(20) << "|Total Checkouts" << setw(20) << "|Total Renewals" << setw(20) << "|Home Library" << setw(20) << "|Notification Medium" << setw(25) << "|Provided Email Address?" << setw(20) << "|Circulation Year" << setw(20) << "|Circulation Month" << setw(20) << "|In SF County?" << setw(20) << "|Year Registered     |" << endl;
    cout << "_______________________________________________________________________________________________________________________________________________________________________________________________________________________________________________________" << endl;
    cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    while (row->next())
    {
        cout << left << "|" << setw(19) << row->getInt("id") << "|" << setw(19) << row->getString("patron_type_definition") << "|" << setw(19) << row->getString("patron_age_range") << "|" << setw(19) << row->getInt("checkout_total") << "|" << setw(19) << row->getInt("renewal_total") << "|" << setw(19) << row->getString("home_library_definition") << "|" << setw(19) << row->getString("notification_code_definition") << "|" << setw(24) << ((row->getBoolean("provided_email_address")) ? "Yes" : "No") << "|" << setw(19) << row->getInt("circulation_active_year") << "|" << setw(19) << row->getString("circulation_active_month") << "|" << setw(19) << ((row->getBoolean("within_sf_county")) ? "Yes" : "No") << "|" << setw(20) << row->getInt("year_patron_registered") << "|" << endl;
        cout << "-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------" << endl;
    }
}

void Database::totalCheckoutsByPatronType()
{
    sql::Statement* command = myConnection->createStatement();
    sql::ResultSet* row = command->executeQuery("SELECT p.patron_type_definition, SUM(u.checkout_total) total "
                                                "FROM sfpl_usage u "
                                                "JOIN patron_type p "
                                                "ON p.patron_type_code = u.patron_type_code "
                                                "GROUP BY p.patron_type_definition "
                                                "ORDER BY total DESC");
    while (row->next())
    {
        cout << row->getString("patron_type_definition") << ": " << row->getInt("total") << endl;
    }
}

void Database::totalCheckoutsByLibrary()
{
    sql::Statement* command = myConnection->createStatement();
    sql::ResultSet* row = command->executeQuery("SELECT p.home_library_definition, SUM(u.checkout_total) total "
                                                "FROM sfpl_usage u "
                                                "JOIN home_library p "
                                                "ON p.home_library_code = u.home_library_code "
                                                "GROUP BY p.home_library_definition "
                                                "ORDER BY total DESC");
    while (row->next())
    {
        cout << row->getString("home_library_definition") << ": " << row->getInt("total") << endl;
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
