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

        myConnection = driver->connect("tcp://localhost:3306", "guest_user_sfpl", "mypassword123");

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
    sql::ResultSet* row = command->executeQuery("SELECT * FROM patron_type");

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

void Database::viewAgeRangeOptions()
{
    sql::Statement* command = myConnection->createStatement();
    sql::ResultSet* row = command->executeQuery("SELECT DISTINCT patron_age_range "
                                                "FROM sfpl_usage "
                                                "ORDER BY patron_age_range ASC");
    
    int x = 1;
    cout << "Age Range Options: " << endl;
    cout << endl;
    while (row->next() && x < 12)
    {
        cout << x << ". " << ((row->getString("patron_age_range")=="") ? "'empty'" : row->getString("patron_age_range")) << endl;
        x++;
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

void Database::PatronTypeData()
{
    sql::Statement* command = myConnection->createStatement();
    sql::ResultSet* row = command->executeQuery("SELECT p.patron_type_definition, SUM(u.checkout_total) c_tot, SUM(u.renewal_total) r_tot "
                                                "FROM sfpl_usage u "
                                                "JOIN patron_type p "
                                                "ON p.patron_type_code = u.patron_type_code "
                                                "GROUP BY p.patron_type_definition "
                                                "ORDER BY c_tot DESC");
    cout << "_____________________________________________________________" << endl;
    cout << left << setw(20) << "|Patron Type" << setw(20) << "|Checkout Totals" << setw(20) << "|Renewal Totals     |" << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << endl;
    cout << "-------------------------------------------------------------" << endl;
    while (row->next())
    {
        cout << left << "|" << setw(19) << row->getString("patron_type_definition") << "|" << setw(19) << row->getInt("c_tot") << "|" << setw(19) << row->getInt("r_tot") << "|" << endl;
        cout << "-------------------------------------------------------------" << endl;
    }
}

void Database::LibraryData()
{
    sql::Statement* command = myConnection->createStatement();
    sql::ResultSet* row = command->executeQuery("SELECT p.home_library_definition, SUM(u.checkout_total) c_tot, SUM(u.renewal_total) r_tot "
                                                "FROM sfpl_usage u "
                                                "JOIN home_library p "
                                                "ON p.home_library_code = u.home_library_code "
                                                "GROUP BY p.home_library_definition "
                                                "ORDER BY c_tot DESC");
    cout << "_____________________________________________________________" << endl;
    cout << left << setw(20) << "|Home Library" << setw(20) << "|Checkout Totals" << setw(20) << "|Renewal Totals     |" << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << endl;
    cout << "-------------------------------------------------------------" << endl;
    while (row->next())
    {
        cout << left << "|" << setw(19) << row->getString("home_library_definition") << "|" << setw(19) << row->getInt("c_tot") << "|" << setw(19) << row->getInt("r_tot") << "|" << endl;
        cout << "-------------------------------------------------------------" << endl;
    }
}

void Database::addRecord()
{
    int patron_age_range, input_checkouts, input_renewals, circ_act_year, within_SF_County, year_registered;
    string patron_type_code, age_range, home_library_code, notification_medium_code, circ_act_month;
    string ans;

    cout << "Show Patron options? (y/n) ";
    cin >> ans;
    if (ans == "y") { cout << endl; viewPatronTypes(); cout << endl; }
    cout << "Enter Patron Type CODE: ";
    cin >> patron_type_code;
    cout << endl;

    cout << "Show Age Range options? (y/n) ";
    cin >> ans;
    if (ans == "y") { cout << endl; viewAgeRangeOptions(); cout << endl; }

    do
    {
        cout << "Enter Patron Age Range #: ";
        cin >> patron_age_range;

        switch (patron_age_range)
        {
        case (1):
            age_range = "";
            break;
        case (2):
            age_range = "0 to 9 years";
            break;
        case (3):
            age_range = "10 to 19 years";
            break;
        case (4):
            age_range = "20 to 24 years";
            break;
        case (5):
            age_range = "25 to 34 years";
            break;
        case (6):
            age_range = "35 to 44 years";
            break;
        case (7):
            age_range = "45 to 54 years";
            break;
        case (8):
            age_range = "55 to 59 years";
            break;
        case (9):
            age_range = "60 to 64 years";
            break;
        case (10):
            age_range = "65 to 74 years";
            break;
        case (11):
            age_range = "75 years and over";
            break;
        default:
            age_range = "";
            cout << "Invalid number" << endl;
            break;
        }
    } while (age_range == "");
    cout << endl;

    cout << "Enter Checkout Total (0 for default): ";
    cin >> input_checkouts;
    cout << endl;

    cout << "Enter Renewal Total (0 for default): ";
    cin >> input_renewals;
    cout << endl;

    cout << "Show Library options? (y/n) ";
    cin >> ans;
    if (ans == "y") { cout << endl; viewLibraries(); cout << endl; }
    cout << "Enter Home Library CODE: ";
    cin >> home_library_code;
    cout << endl;

    cout << "Show Notification Medium options? (y/n) ";
    cin >> ans;
    if (ans == "y") { cout << endl; viewNotificationMediums(); cout << endl; }
    cout << "Enter Notification Medium CODE: ";
    cin >> notification_medium_code;
    cout << endl;

    cout << "Enter Circulation Active Year (0 for NULL): ";
    cin >> circ_act_year;
    cout << endl;

    cout << "Enter Circulation Active Month: ";
    cin >> circ_act_month;
    cout << endl;

    cout << "Within San Francisco County? Enter # (1 for yes, 0 for no): ";
    cin >> within_SF_County;
    cout << endl;

    cout << "Year Registered: ";
    cin >> year_registered;
    cout << endl;

    sql::PreparedStatement* prepCommand = myConnection->prepareStatement("INSERT INTO sfpl_usage "
                                                                         "(patron_type_code, patron_age_range, checkout_total, renewal_total, home_library_code, notification_medium_code, provided_email_address, circulation_active_year, circulation_active_month, within_SF_County, year_patron_registered) "
                                                                         "VALUES(?,?,?,?,?,?,?,?,?,?,?)");

    prepCommand->setString(1, patron_type_code);
    prepCommand->setString(2, age_range);
    prepCommand->setInt(3, input_checkouts);
    prepCommand->setInt(4, input_renewals);
    prepCommand->setString(5, home_library_code);
    prepCommand->setString(6, notification_medium_code);
    prepCommand->setInt(7, ((notification_medium_code == "z") ? 1 : 0));
    prepCommand->setInt(8, circ_act_year);
    prepCommand->setString(9, circ_act_month);
    prepCommand->setInt(10, within_SF_County);
    prepCommand->setInt(11, year_registered);

    prepCommand->execute();

    sql::Statement* command = myConnection->createStatement();
    sql::ResultSet* row = command->executeQuery("SELECT LAST_INSERT_ID()");

    cout << endl;
    if (row->next())
    {
        int newId = row->getInt(1);
        cout << "Record added with ID: " << newId << endl;
    }
}

void Database::updateRecord()
{
    int id, patron_age_range, input_checkouts, input_renewals, circ_act_year, within_SF_County, year_registered;
    string patron_type_code, age_range, home_library_code, notification_medium_code, circ_act_month;
    string ans;

    cout << "Enter ID: ";
    cin >> id;
    cout << endl;

    cout << "Show Patron options? (y/n) ";
    cin >> ans;
    if (ans == "y") { cout << endl; viewPatronTypes(); cout << endl; }
    cout << "Enter Patron Type CODE: ";
    cin >> patron_type_code;
    cout << endl;

    cout << "Show Age Range options? (y/n) ";
    cin >> ans;
    if (ans == "y") { cout << endl; viewAgeRangeOptions(); cout << endl; }

    do
    {
    cout << "Enter Patron Age Range #: ";
    cin >> patron_age_range;

        switch (patron_age_range)
        {
        case (1):
            age_range = "";
            break;
        case (2):
            age_range = "0 to 9 years";
            break;
        case (3):
            age_range = "10 to 19 years";
            break;
        case (4):
            age_range = "20 to 24 years";
            break;
        case (5):
            age_range = "25 to 34 years";
            break;
        case (6):
            age_range = "35 to 44 years";
            break;
        case (7):
            age_range = "45 to 54 years";
            break;
        case (8):
            age_range = "55 to 59 years";
            break;
        case (9):
            age_range = "60 to 64 years";
            break;
        case (10):
            age_range = "65 to 74 years";
            break;
        case (11):
            age_range = "75 years and over";
            break;
        default:
            age_range = "";
            cout << "Invalid number" << endl;
            break;
        }
    } while (age_range=="");
    cout << endl;

    cout << "Enter Checkout Total (0 for default): ";
    cin >> input_checkouts;
    cout << endl;

    cout << "Enter Renewal Total (0 for default): ";
    cin >> input_renewals;
    cout << endl;
    
    cout << "Show Library options? (y/n) ";
    cin >> ans;
    if (ans == "y") { cout << endl; viewLibraries(); cout << endl; }
    cout << "Enter Home Library CODE: ";
    cin >> home_library_code;
    cout << endl;

    cout << "Show Notification Medium options? (y/n) ";
    cin >> ans;
    if (ans == "y") { cout << endl; viewNotificationMediums(); cout << endl; }
    cout << "Enter Notification Medium CODE: ";
    cin >> notification_medium_code;
    cout << endl;

    cout << "Enter Circulation Active Year (0 for NULL): ";
    cin >> circ_act_year;
    cout << endl;

    cout << "Enter Circulation Active Month: ";
    cin >> circ_act_month;
    cout << endl;

    cout << "Within San Francisco County? Enter # (1 for yes, 0 for no): ";
    cin >> within_SF_County;
    cout << endl;

    cout << "Year Registered: ";
    cin >> year_registered;
    cout << endl;

    sql::PreparedStatement* prepCommand = myConnection->prepareStatement("UPDATE sfpl_usage "
                                                                         "SET patron_type_code=?, "
                                                                             "patron_age_range=?, "
                                                                             "checkout_total=?, "
                                                                             "renewal_total=?, "
                                                                             "home_library_code=?, "
                                                                             "notification_medium_code=?, "
                                                                             "provided_email_address=?, "
                                                                             "circulation_active_year=?, "
                                                                             "circulation_active_month=?, "
                                                                             "within_SF_County=?, "
                                                                             "year_patron_registered=? "
                                                                         "WHERE id=?");

    prepCommand->setString(1, patron_type_code);
    prepCommand->setString(2, age_range);
    prepCommand->setInt(3, input_checkouts);
    prepCommand->setInt(4, input_renewals);
    prepCommand->setString(5, home_library_code);
    prepCommand->setString(6, notification_medium_code);
    prepCommand->setInt(7, ((notification_medium_code=="z") ? 1 : 0));
    prepCommand->setInt(8, circ_act_year);
    prepCommand->setString(9, circ_act_month);
    prepCommand->setInt(10, within_SF_County);
    prepCommand->setInt(11, year_registered);
    prepCommand->setInt(12, id);

    prepCommand->execute();
}

void Database::deleteRecord()
{
    int input_id;

    cout << "Enter ID: ";
    cin >> input_id;

    sql::PreparedStatement* prepCommand = myConnection->prepareStatement("DELETE FROM sfpl_usage "
                                                                         "WHERE id=?");

    prepCommand->setInt(1, input_id);

    prepCommand->execute();
}
