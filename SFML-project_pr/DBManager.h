#pragma once
#include <stdlib.h>
#include <iostream>
#include <string>
#include <memory>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "cppconn/metadata.h"
#include <map>
#include "mysql_driver.h"
#include "cppconn/statement.h"

using namespace std;



class DBManager {
public:
	DBManager() {

		const string server = "tcp://127.0.0.1:3306";
		const string username = "root";
		const string password = "Root17!_";

		sql::Driver* driver;


		try
		{
			driver = get_driver_instance();
			con = driver->connect(server, username, password);
		}
		catch (sql::SQLException e)
		{
			cout << "Could not connect to server. Error message: " << e.what() << endl;
			system("pause");
			exit(1);
		}
		auto dbcon_meta = con->getMetaData();
		
		auto rs = dbcon_meta->getSchemas();
		int row = 1;

		while (rs->next()) {
			cout << "\t" << row << ". " << rs->getString("TABLE_SCHEM") << endl;
			++row;
		} // while

		/*try {
			con->setSchema("quickstartdb");
		}
		catch (sql::SQLException e)
		{
			cout << e.what() << endl;
			system("pause");
			exit(1);
		}

		stmt = con->createStatement();*/
	};
private:
	sql::Connection* con;

};