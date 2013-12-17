/*
 * mysql.h
 *
 *  Created on: 2013-2-7
 *      Author: purple
 */

#ifndef MYSQL_H_
#define MYSQL_H_

#include <iostream>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include "Base.h"
#include "Status.h"

using namespace sql;
using namespace std;

class Mysql : public Base{
public:
	Mysql();
	~Mysql();
	string getCode(int statusID);
	void update_status(Status *status);
	void saveStatusInfo(Status *status);
	void saveStatusCEreason(Status *status);
	void saveStatusTimeAndMemory(Status *status);
	void update_problem_submit(int problemID);
	void update_contest_submit(Status *status);
private:
	Driver *driver;
	Connection *conn;
	ResultSet *rst;
	static string user;
	static string pswd;
	static string host;
	void findByProblemID(int problemID);
	void findByStatusID(int statusID);
	void findByContestID(int statusID);
	void connector();
};


#endif /* MYSQL_H_ */
