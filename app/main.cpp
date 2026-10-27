#include <iostream>
#include "Database.h"
using namespace std;

int main()
{
    Database my_db;
    int input;

    do
    {
        cout << "-------------------------------------------------------" << endl;
        cout << "C++ Interface for San Francisco Public Library Database" << endl;
        cout << "-------------------------------------------------------" << endl;
        cout << "1. View Patron Types Options" << endl;
        cout << "2. View Libraries Options" << endl;
        cout << "3. View Notification Mediums Options" << endl;
        cout << "4. View Age Range Options" << endl;
        cout << "5. Search by ID" << endl;
        cout << "6. Patron Type Data" << endl;
        cout << "7. Home Library Data" << endl;
        cout << "8. Add Record" << endl;
        cout << "9. Update Record" << endl;
        cout << "10. Delete Record" << endl;
        cout << "11. Exit Program" << endl;
        cout << "--------------------------------------------------------" << endl;

        cout << "Enter Option #: ";
        cin >> input;
        cout << endl;

        switch (input)
        {
        case (1):
            my_db.viewPatronTypes();
            break;
        case (2):
            my_db.viewLibraries();
            break;
        case (3):
            my_db.viewNotificationMediums();
            break;
        case (4):
            my_db.viewAgeRangeOptions();
            break;
        case (5):
            my_db.searchByID();
            break;
        case (6):
            my_db.PatronTypeData();
            break;
        case (7):
            my_db.LibraryData();
            break;
        case (8):
            my_db.addRecord();
            break;
        case (9):
            my_db.updateRecord();
            break;
        case (10):
            my_db.deleteRecord();
            break;
        case (11):
            cout << "Exiting" << endl;
            break;
        default:
            cout << "Invalid number" << endl;
            break;
        }

        cout << endl;
        cout << "Press Enter to Continue..." << endl;
        cin.ignore();
        cin.get();
    } while (input != 11);

    return 0;
}
