#include <iostream>
#include "Database.h"
using namespace std;

int main()
{
  Database my_db;
  int input;

  do
  {
    cout << "---------------------" << endl;
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
    cout << "11. Exit Program" << endl;
    cout << "---------------------" << endl;

    cin >> input;

    switch(input)
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
    }
  } while (input != 11);

  return 0;
}
