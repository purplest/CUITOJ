/*
 * Program.h
 *
 *  Created on: 2013-2-7
 *      Author: purple
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "Log.h"
#include "Mysql.h"
#include "Compile.h"
#include "Status.h"
#include "Base.h"
#include "Run.h"

#ifndef PROGRAM_H_
#define PROGRAM_H_

class Program : public Base {
public:
	Program();
	~Program();
	void setStatus(Status *status);
	void start();
private:
	void saveState();
	void saveCEreason();
	void saveTimeAndMemory();
	Compile *compile;
	Status *status;
	Mysql *mysql;
	Run *run;

	void prepare();
};


#endif /* PROGRAM_H_ */
