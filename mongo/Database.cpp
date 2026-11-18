#include "Database.h"
#include <iomanip>
#include <set>
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

    set<string> patronTypes;

    for (auto&& doc : cursor) {
        patronTypes.insert(doc["patron_type_definition"].get_string().value.to_string());
    }

    for (auto& type : patronTypes) {
        cout << type << endl;
    }
}

void Database::viewLibraries()
{
    auto cursor = myTable.find({});

    set<string> libraries;

    for (auto&& doc : cursor) {
        libraries.insert(doc["home_library_definition"].get_string().value.to_string());
    }

    for (auto& lib : libraries) {
        cout << lib << endl;
    }
}

void Database::viewNotificationMediums()
{
    auto cursor = myTable.find({});

    set<string> mediums;

    for (auto&& doc : cursor) {
        mediums.insert(doc["notification_medium_definition"].get_string().value.to_string());
    }

    for (auto& medium : mediums) {
        cout << medium << endl;
    }
}

void Database::viewAgeRangeOptions()
{
    auto cursor = myTable.find({});
    
    set<string> ranges;

    for (auto&& doc : cursor) {
        if(doc["age_range"]){
            ranges.insert(doc["age_range"].get_string().value.to_string()
            );
        }
    }

    int count = 1;

    for(auto& range : ranges) {
        cout << count++ << ". " << range << endl;
    }
}

void Database::searchByID()
{
    int id;

    cout << "Enter ID: ";
    cin >> id;

    auto result = myTable.find_one(make_document(kvp("id", id)));

    if (!result){
        cout << "No record found." << endl;
        return;
    }

    auto doc = result->view();

    cout << endl;

    cout << "ID: " << doc["id"].get_int32() << endl;

    cout << "Patron Type: " << doc["patron_type_definition"].get_string().value.to_string() << endl;

    cout << "Age Range: " << doc["age_range"].get_string().value.to_string() << endl;

    cout << "Checkouts: " << doc["checkout_total"].get_int32() << endl;

    cout << "Renewals: " << doc["renewal_total"].get_int32() << endl;
}

void Database::PatronTypeData()
{
    mongocxx::pipeline p;

    p.group(make_document(kvp("_id", "$patron_type_definition"), kvp("total_checkouts", make_document(kvp("$sum", "$checkout_total"))), kvp("total_renewals", make_document(kvp("$sum", "$renewal_total")))));

    auto cursor = myTable.aggregate(p);

    for (auto&& doc : cursor)
    {
        cout << doc["_id"].get_string().value.to_string() << " | Checkouts: " << doc["total_checkouts"].get_int32() << " | Renewals: " << doc["total_renewals"].get_int32() << endl;
    }
}

void Database::LibraryData()
{
    mongocxx::pipeline p;

    p.group(make_document(kvp("_id", "$home_library_definition"), kvp("total_checkouts", make_document(kvp("$sum", "$checkout_total")))));

    auto cursor = myTable.aggregate(p);

    for (auto&& doc : cursor) {
        cout << doc["_id"].get_string().value.to_string() << " -> " << doc["total_checkouts"].get_int32() << endl;
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
