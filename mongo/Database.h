#pragma once

#include <iostream>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/collection.hpp>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
mongocxx::instance Database::instance{};

class Database
{
private:
    static mongocxx::instance instance;
    mongocxx::client myConnection;
    mongocxx::database myDB;
    mongocxx::collection myTable;
public:
    Database();
    void viewPatronTypes();
    void viewLibraries();
    void viewNotificationMediums();

    void searchByID();

    void PatronTypeData();
    void LibraryData();

    void addRecord();
    void updateRecord();
    void deleteRecord();
};
