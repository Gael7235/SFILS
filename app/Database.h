#pragma once

#include <mysql_driver.h>
#include <mysql_connections.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
using namespace sql;

class Database
{
private:
  Connection* myConnection;
public:
  Database();
  void viewPatronTypes();
};
