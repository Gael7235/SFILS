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
        patronTypes.insert({string(doc["patron_type_code"].get_string().value), string(doc["patron_type_definition"].get_string().value)});
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
    cout << "Provided Email Address?: " << (doc["provided_email_address"].get_bool() ? "Yes" : "No") << endl;
    cout << "Circulation Year: " << string(doc["circulation_active_year"].get_string().value) << endl;
    cout << "Circulation Month: " << string(doc["circulation_active_month"].get_string().value) << endl;
    cout << "In SF County?: " << (doc["within_sf_county"].get_bool() ? "Yes" : "No") << endl;
    cout << "Year Registered: " << string(doc["year_patron_registered"].get_string().value) << endl;
}

void Database::PatronTypeData()
{
    mongocxx::pipeline p;

    p.group(make_document(kvp("_id", "$patron_type_definition"), kvp("total_checkouts", make_document(kvp("$sum", "$checkout_total"))), kvp("total_renewals", make_document(kvp("$sum", "$renewal_total")))));

    auto cursor = myTable.aggregate(p);

    for (auto&& doc : cursor)
    {
        cout << string(doc["_id"].get_string().value) << " | Checkouts: " << doc["total_checkouts"].get_int32().value << " | Renewals: " << doc["total_renewals"].get_int32().value << endl;
    }
}

void Database::LibraryData()
{
    mongocxx::pipeline p;

    p.group(make_document(kvp("_id", "$home_library_definition"), kvp("total_checkouts", make_document(kvp("$sum", "$checkout_total")))));

    auto cursor = myTable.aggregate(p);

    for (auto&& doc : cursor) {
        cout << string(doc["_id"].get_string().value) << " -> " << doc["total_checkouts"].get_int32().value << endl;
    }
}

void Database::addRecord()
{
    int id;
    int checkouts;
    int renewals;

    string patronType;
    string ageRange;
    string library;

    cout << "ID: ";
    cin >> id;

    cin.ignore();

    cout << "Patron Type: ";
    getline(cin, patronType);

    cout << "Age Range: ";
    getline(cin, ageRange);

    cout << "Checkout Total: ";
    cin >> checkouts;

    cout << "Renewal Total: ";
    cin >> renewals;

    cin.ignore();

    cout << "Library: ";
    getline(cin, library);

    auto document = make_document(kvp("id", id), kvp("patron_type_definition", patronType), kvp("age_range", ageRange), kvp("checkout_total", checkouts), kvp("renewal_total", renewals), kvp("home_library_definition", library));

    myTable.insert_one(document.view());
}

void Database::updateRecord()
{
    int id;
    int newCheckouts;

    cout << "Enter ID: ";
    cin >> id;

    cout << "New Checkout Total: ";
    cin >> newCheckouts;

    myTable.update_one(make_document(kvp("id", id)), make_document(kvp("$set", make_document(kvp("checkout_total", newCheckouts)))));
}

void Database::deleteRecord()
{
    int id;

    cout << "Enter ID: ";
    cin >> id;

    myTable.delete_one(make_document(kvp("id", id)));
}
