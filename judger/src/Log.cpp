/*
 * Log.cpp
 *
 *  Created on: 2013-5-11
 *      Author: purple
 */

#include "Log.h"

using namespace std;

Log * Log::instance = NULL;

Log::Log(){

}

Log::~Log(){

}

Log * Log::getInstance(){
	if (!instance)
		instance = new Log;
	return instance;
}

void Log::writelog(char *log){
	FILE *fp = fopen(this->filename.c_str(),"a");
	if (NULL!=fp){
		time_t now_time;
		time(&now_time);
		tm *now = localtime(&now_time);
		fprintf(fp, "%d-%02d-%02d %02d:%02d:%02d ", now->tm_year+1900, now->tm_mon, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec);
		fputs(log,fp);
		fputs("\n",fp);
		fclose(fp);
	}
}

void Log::writelog(const char *log){
	this->writelog((char *)log);
}

void Log::writelog(string log){
	this->writelog(log.c_str());
}
