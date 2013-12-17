/*
 * Compile.h
 *
 *  Created on: 2013-2-7
 *      Author: purple
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <dirent.h>

#include "Log.h"
#include "Status.h"
#include "Base.h"

#ifndef COMPILE_H_
#define COMPILE_H_

using namespace std;

class Compile : public Base {
public:
	Compile(Status *status);
	~Compile();
	void process();
private:
	void setState();
	void clearPath();
	bool checkfile();
	Status *status;
	static const int compile_time_limited = 10;
	int compile_time_used;
	string c_arg, java_arg;
};

#endif /* COMPILE_H_ */
