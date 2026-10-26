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
        cout << "1. View Patron Types" << endl;
        cout << "2. View Libraries" << endl;
        cout << "3. View Notification Mediums" << endl;
        cout << "4. Search by Patron Type" << endl;
        cout << "5. Search by Home Library" << endl;
        cout << "6. Total Checkouts by Patron Type" << endl;
        cout << "7. Total Checkouts by Library" << endl;
        cout << "8. Add Record" << endl;
        cout << "9. Update Record" << endl;
        cout << "10. Delete Record" << endl;
        cout << "11. Search by ID" << endl;
        cout << "12. Exit Program" << endl;
        cout << "--------------------------------------------------------" << endl;

        cout << "Enter Option: ";
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
            my_db.searchPatronType();
            break;
        case (5):
            my_db.searchLibrary();
            break;
        case (6):
            my_db.totalCheckoutsByPatronType();
            break;
        case (7):
            my_db.totalCheckoutsByLibrary();
            break;
        case (8):
            my_db.addUsageRecord();
            break;
        case (9):
            my_db.updateUsageRecord();
            break;
        case (10):
            my_db.deleteUsageRecord();
            break;
        case (11):
            my_db.searchByID();
            break;
        case (12):
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
    } while (input != 12);

    return 0;
}
