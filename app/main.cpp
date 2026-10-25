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
    cout << "2. " << endl;
    cout << "3. " << endl;
    cout << "4. " << endl;
    cout << "5. " << endl;
    cout << "6. " << endl;
    cout << "7. " << endl;
    cout << "8. " << endl;
    cout << "9. " << endl;
    cout << "10. " << endl;
    cout << "---------------------" << endl;

    cin >> input;

    switch(input)
    {
      case (1):
        viewPatronTypes();
        break;
      case (2):
        //do
        break;
      case (3):
        //do
        break;
      case (4):
        //do
        break;
      case (5):
        //do
        break;
      case (6):
        //do
        break;
      case (7):
        //do
        break;
      case (8):
        //do
        break;
      case (9):
        //do
        break;
    }
  } while (input != 10);

  return 0;
}
