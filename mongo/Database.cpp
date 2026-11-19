#include "Database.h"
#include <iomanip>
#include <set>
#include <string>
mongocxx::instance Database::instance{};

using namespace std;

Database::Database() : myConnection(mongocxx::uri("mongodb://localhost:27017"))
{
    try {
        // Connect to DB
        myDB = myConnection["SFPL_DB"];
        // Connect to Collection
        myTable = myDB["SFPL_Table"];
    }
    // Connection unsuccesful
    catch (const exception& e) {
        std::cerr << "Error: " << e.what() << endl;
    }
}

void Database::viewPatronTypes()
{
    auto cursor = myTable.find({});
    set<pair<string, string>> patronTypes;

    // Stores wanted items (as {code, definition})
    for (auto&& doc : cursor) {
        patronTypes.insert({to_string(doc["patron_type_code"].get_int32().value), string(doc["patron_type_definition"].get_string().value)});
    }

    // Outputs stored items (as code - definition)
    for (auto& type : patronTypes) {
        cout << type.first << " - " << type.second << endl;
    }
}

void Database::viewLibraries()
{
    auto cursor = myTable.find({});
    set<pair<string, string>> libraries;

    // Stores wanted items (as {code, definition})
    for (auto&& doc : cursor) {
        libraries.insert({string(doc["home_library_code"].get_string().value), string(doc["home_library_definition"].get_string().value)});
    }

    // Outputs stored items (as code - definition)
    for (auto& lib : libraries) {
        cout << lib.first << " - " << lib.second << endl;
    }
}

void Database::viewNotificationMediums()
{
    auto cursor = myTable.find({});
    set<pair<string, string>> mediums;

    // Stores wanted items (as {code, definition})
    for (auto&& doc : cursor) {
        mediums.insert({string(doc["notification_medium_code"].get_string().value), string(doc["notification_code_definition"].get_string().value)});
    }

    // Outputs stored items (as code - definition)
    for (auto& medium : mediums) {
        cout << medium.first << " - " << medium.second << endl;
    }
}

void Database::viewAgeRangeOptions()
{
    auto cursor = myTable.find({});
    set<string> ranges;

    // Stores wanted items
    for (auto&& doc : cursor) {
        if (doc["age_range"]) {
            ranges.insert(string(doc["age_range"].get_string().value));
        }
    }

    // Outputs stored items
    int x = 1;
    cout << "Age Range Options: " << endl;
    cout << endl;
    for(auto& range : ranges) {
        cout << x << ". " << ((range=="") ? "empty" : range) << endl;
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

    auto result = myTable.find_one(make_document(kvp("id", int_input)));

    // If we didn't find an entry
    if (!result) {cout << "Failed" << endl; return;}
    
    auto doc = result->view();

    cout << endl;
    cout << "ID: " << doc["id"].get_int32().value << endl;
    cout << "Patron Type: " << string(doc["patron_type_definition"].get_string().value) << endl;
    cout << "Age Range: " << string(doc["age_range"].get_string().value) << endl;
    cout << "Total Checkouts: " << doc["checkout_total"].get_int32().value << endl;
    cout << "Total Renewals: " << doc["renewal_total"].get_int32().value << endl;
    cout << "Home Library: " << string(doc["home_library_definition"].get_string().value) << endl;
    cout << "Notification Medium: " << string(doc["notification_code_definition"].get_string().value) << endl;
    cout << "Provided Email Address?: " << (doc["provided_email_address"].get_bool().value ? "Yes" : "No") << endl;
    cout << "Circulation Year: " << string(doc["circulation_active_year"].get_string().value) << endl;
    cout << "Circulation Month: " << string(doc["circulation_active_month"].get_string().value) << endl;
    cout << "In SF County?: " << (doc["within_sf_county"].get_bool().value ? "Yes" : "No") << endl;
    cout << "Year Registered: " << string(doc["year_patron_registered"].get_string().value) << endl;
}

void Database::PatronTypeData()
{
    mongocxx::pipeline p;

    p.group(make_document(kvp("_id", "$patron_type_definition"), kvp("c_tot", make_document(kvp("$sum", "$checkout_total"))), kvp("r_tot",make_document(kvp("$sum", "$renewal_total")))));
    p.sort(make_document(kvp("c_tot", -1)));

    auto cursor = myTable.aggregate(p);

    cout << "_____________________________________________________________" << endl;
    cout << left << setw(20) << "|Patron Type" << setw(20) << "|Checkout Totals" << setw(20) << "|Renewal Totals     |" << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << endl;
    cout << "-------------------------------------------------------------" << endl;

    while (auto&& doc : cursor) {
        cout << left << "|" << setw(19) << string(doc["_id"].get_string().value) << "|" << setw(19) << doc["c_tot"].get_int64().value << "|" << setw(19) << doc["r_tot"].get_int64().value << "|" << endl;
        cout << "-------------------------------------------------------------" << endl;
    }
}

void Database::LibraryData()
{
    mongocxx::pipeline p;

    p.group(make_document(kvp("_id", "$home_library_definition"), kvp("c_tot", make_document(kvp("$sum", "$checkout_total"))), kvp("r_tot",make_document(kvp("$sum", "$renewal_total")))));
    p.sort(make_document(kvp("c_tot", -1)));

    auto cursor = myTable.aggregate(p);

    cout << "_____________________________________________________________" << endl;
    cout << left << setw(20) << "|Home Library" << setw(20) << "|Checkout Totals" << setw(20) << "|Renewal Totals     |" << endl;
    cout << "-------------------------------------------------------------" << endl;
    cout << endl;
    cout << "-------------------------------------------------------------" << endl;

    while (auto&& doc : cursor) {
        cout << left << "|" << setw(19) << string(doc["_id"].get_string().value) << "|" << setw(19) << doc["c_tot"].get_int64().value << "|" << setw(19) << doc["r_tot"].get_int64().value << "|" << endl;
        cout << "-------------------------------------------------------------" << endl;
    }
}

void Database::addRecord()
{
    int patron_age_range, input_checkouts, input_renewals, circ_act_year, within_SF_County, year_registered;
    string patron_type_code, patron_type_definition, age_range, home_library_code, home_library_definition, notification_medium_code, notification_code_definition, circ_act_month;
    string ans;

    // Create new ID not in use
    mongocxx::options::find opts;
    opts.sort(make_document(kvp("id", -1)));
    opts.limit(1);
    int nextID = 1;
    auto cursor = myTable.find({}, opts);
    for (auto&& doc : cursor) {
        nextID = doc["id"].get_int32().value + 1;
    }

    // For code
    cout << "Show Patron options? (y/n) ";
    cin >> ans;
    if (ans == "y") { cout << endl; viewPatronTypes(); cout << endl; }
    cout << "Enter Patron Type CODE: ";
    cin >> patron_type_code;
    cout << endl;

    // For definition
    auto patronDoc = myTable.find_one(make_document(kvp("patron_type_code", stoi(patron_type_code))));
    if (patronDoc) {
        patron_type_definition = string(patronDoc->view()["patron_type_definition"].get_string().value);
    }
    

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

    // For code
    cout << "Show Library options? (y/n) ";
    cin >> ans;
    if (ans == "y") { cout << endl; viewLibraries(); cout << endl; }
    cout << "Enter Home Library CODE: ";
    cin >> home_library_code;
    cout << endl;

    // For definition
    auto libraryDoc = myTable.find_one(make_document(kvp("home_library_code", home_library_code)));
    if (libraryDoc) {
        home_library_definition = string(libraryDoc->view()["home_library_definition"].get_string().value);
    }

    // For code
    cout << "Show Notification Medium options? (y/n) ";
    cin >> ans;
    if (ans == "y") { cout << endl; viewNotificationMediums(); cout << endl; }
    cout << "Enter Notification Medium CODE: ";
    cin >> notification_medium_code;
    cout << endl;

    // For definition
    auto notificationDoc = myTable.find_one(make_document(kvp("notification_medium_code", notification_medium_code)));
    if (notificationDoc) {
        notification_code_definition = string(notificationDoc->view()["notification_code_definition"].get_string().value);
    }

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

    auto document = make_document(kvp("id", nextID), kvp("patron_type_code", stoi(patron_type_code)), kvp("patron_type_definition", patron_type_definition), kvp("age_range", age_range), kvp("checkout_total", input_checkouts), kvp("renewal_total", input_renewals), kvp("home_library_code", home_library_code), kvp("home_library_definition", home_library_definition), kvp("notification_medium_code", notification_medium_code), kvp("notification_code_definition", notification_code_definition), kvp("provided_email_address", (notification_medium_code == "z")), kvp("circulation_active_year", to_string(circ_act_year)), kvp("circulation_active_month", circ_act_month), kvp("within_sf_county", within_SF_County == 1), kvp("year_patron_registered", to_string(year_registered)));
    myTable.insert_one(document.view());
}

void Database::updateRecord()
{
    int id, patron_age_range, input_checkouts, input_renewals, circ_act_year, within_SF_County, year_registered;
    string patron_type_code, patron_type_definition, age_range, home_library_code, home_library_definition, notification_medium_code, notification_code_definition, circ_act_month;
    string ans;

    cout << "Enter ID: ";
    cin >> id;
    cout << endl;

     // Check if id exists
    auto result = myTable.find_one(make_document(kvp("id", id)));
    // If we didn't find an entry
    if (!result) {cout << "Failed" << endl; return;}

    // For code
    cout << "Show Patron options? (y/n) ";
    cin >> ans;
    if (ans == "y") { cout << endl; viewPatronTypes(); cout << endl; }
    cout << "Enter Patron Type CODE: ";
    cin >> patron_type_code;
    cout << endl;

    // For definition
    auto patronDoc = myTable.find_one(make_document(kvp("patron_type_code", stoi(patron_type_code))));
    if (patronDoc) {
        patron_type_definition = string(patronDoc->view()["patron_type_definition"].get_string().value);
    }
    

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

    // For code
    cout << "Show Library options? (y/n) ";
    cin >> ans;
    if (ans == "y") { cout << endl; viewLibraries(); cout << endl; }
    cout << "Enter Home Library CODE: ";
    cin >> home_library_code;
    cout << endl;

    // For definition
    auto libraryDoc = myTable.find_one(make_document(kvp("home_library_code", home_library_code)));
    if (libraryDoc) {
        home_library_definition = string(libraryDoc->view()["home_library_definition"].get_string().value);
    }

    // For code
    cout << "Show Notification Medium options? (y/n) ";
    cin >> ans;
    if (ans == "y") { cout << endl; viewNotificationMediums(); cout << endl; }
    cout << "Enter Notification Medium CODE: ";
    cin >> notification_medium_code;
    cout << endl;

    // For definition
    auto notificationDoc = myTable.find_one(make_document(kvp("notification_medium_code", notification_medium_code)));
    if (notificationDoc) {
        notification_code_definition = string(notificationDoc->view()["notification_code_definition"].get_string().value);
    }

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

    myTable.update_one(make_document(kvp("id", id)), make_document(kvp("$set", make_document(kvp("patron_type_code", stoi(patron_type_code)), kvp("patron_type_definition", patron_type_definition), kvp("age_range", age_range), kvp("checkout_total", input_checkouts), kvp("renewal_total", input_renewals), kvp("home_library_code", home_library_code), kvp("home_library_definition", home_library_definition), kvp("notification_medium_code", notification_medium_code), kvp("notification_code_definition", notification_code_definition), kvp("provided_email_address", (notification_medium_code == "z")), kvp("circulation_active_year", to_string(circ_act_year)), kvp("circulation_active_month", circ_act_month), kvp("within_sf_county", within_SF_County == 1), kvp("year_patron_registered", to_string(year_registered))))));
}

void Database::deleteRecord()
{
    string input;

    cout << "Enter ID: ";
    cin.ignore();
    getline(cin, input);

    int int_input = stoi(input);

    myTable.delete_one(make_document(kvp("id", int_input)));
}
