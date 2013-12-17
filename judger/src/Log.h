/*
 * Log.h
 *
 *  Created on: 2013-5-11
 *      Author: purple
 */
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>
#include <time.h>

#include "Base.h"

#define LOG Log::getInstance()->writelog

#ifndef LOG_H_
#define LOG_H_

using namespace std;

class Log : public Base {
public:
    static Log* getInstance();
    void writelog(char* log);
    void writelog(const char* log);
    void writelog(string log);

    string getFilename() const
    {
        return filename;
    }

    void setFilename(string filename)
    {
        this->filename = filename;
    }

private:
    Log();
    ~Log();
    string filename;
    static Log* instance;
};

#endif /* LOG_H_ */
