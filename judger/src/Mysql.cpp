/*
 * mysql.cpp
 *
 *  Created on: 2013-2-6
 *      Author: purple
 */
#include "Mysql.h"

using namespace sql;
using namespace std;

string Mysql::user="root";
string Mysql::pswd="root";
string Mysql::host="tcp://127.0.0.1:3306";

Mysql::Mysql(){
	this->connector();
}

Mysql::~Mysql(){
	delete this->conn;
}

void Mysql::connector(){
	this->driver = get_driver_instance();
	try {
		this->conn = this->driver->connect(this->host,this->user,this->pswd);
		this->conn->setSchema("OnlineJudge");
	} catch (SQLException &e) {
		cout << e.getErrorCode() << endl;
		cout << e.getSQLState() << endl;
	}
	//cout<<"has bean conncet"<<endl;
}

void Mysql::findByStatusID(int statusID){
	string sql = "select * from status where status_id = ? ";
	PreparedStatement *pstmt = this->conn->prepareStatement(sql.c_str());
	pstmt->setInt(1,statusID);
	this->rst = pstmt->executeQuery();
}

void Mysql::findByProblemID(int problemID){
	string sql = "select * from problem where problem_id = ? ";
	PreparedStatement *pstmt = this->conn->prepareStatement(sql.c_str());
	pstmt->setInt(1,problemID);
	this->rst = pstmt->executeQuery();
}

void Mysql::update_problem_submit(int problemID){
	string sql = "update problem set problem_AC=problem_AC+1, problem_ALL=problem_ALL+1 where problem_id = ? ";
	PreparedStatement *pstmt = this->conn->prepareStatement(sql.c_str());
	pstmt->setInt(1,problemID);
	pstmt->executeUpdate();
}

void Mysql::update_contest_submit(Status *status){
	string sql = "update contest_problem set contest_problem_tot_ac=contest_problem_tot_ac+1, contest_problem_tot_submit=contest_problem_tot_submit+1 where contest_id = ? and problem_id = ? ";
	PreparedStatement *pstmt = this->conn->prepareStatement(sql.c_str());
	pstmt->setInt(1,status->getContestId());
	pstmt->setInt(2,status->getProblemId());
	pstmt->executeUpdate();
}

void Mysql::findByContestID(int contestID){
	string sql = "select * from contest where problem_id = ? ";
	PreparedStatement *pstmt = this->conn->prepareStatement(sql.c_str());
	pstmt->setInt(1,contestID);
	this->rst = pstmt->executeQuery();
}

string Mysql::getCode(int statusID){
	this->findByStatusID(statusID);
//	cout<<this->rst->rowsCount()<<endl;
	this->rst->next();
	return this->rst->getString("status_code");
}

void Mysql::update_status(Status *status){
	int statusID=status->getStatusId();
	this->findByStatusID(statusID);
	this->rst->next();
	status->setStatusCode(this->rst->getString("status_code"));
	status->setProblemId(this->rst->getInt("problem_id"));
	status->setLanguage(this->rst->getInt("status_language"));

	this->findByProblemID(status->getProblemId());
	this->rst->next();
	status->setCaseTimeLimited(this->rst->getInt("problem_case_time"));;
	status->setTimeLimited(this->rst->getInt("problem_time"));
	status->setMemoryLimited(this->rst->getInt("problem_memory"));
}

void Mysql::saveStatusInfo(Status *status){
	string sql = "update status set status_result = ? where status_id = ?";
	PreparedStatement *pstmt = this->conn->prepareStatement(sql.c_str());
	pstmt->setInt(1,status->getStatusInfo());
	pstmt->setInt(2,status->getStatusId());
	pstmt->executeUpdate();
}

void Mysql::saveStatusCEreason(Status *status){
	string sql = "update status set status_ce_reason = ? where status_id = ?";
	PreparedStatement *pstmt = this->conn->prepareStatement(sql.c_str());
	pstmt->setString(1,status->getCeReason());
	pstmt->setInt(2,status->getStatusId());
	pstmt->executeUpdate();
}

void Mysql::saveStatusTimeAndMemory(Status *status){
	string sql = "update status set status_time = ?, status_memory = ? where status_id = ?";
		PreparedStatement *pstmt = this->conn->prepareStatement(sql.c_str());
		pstmt->setInt(1,status->getTimeUsed());
		pstmt->setInt(2,status->getMemoryUsed());
		pstmt->setInt(3,status->getStatusId());
		pstmt->executeUpdate();
}
