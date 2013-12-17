/*
 * Base.h
 *
 *  Created on: 2013-2-7
 *      Author: purple
 */
#include <cstdio>
#include <iostream>
#include <string>

#ifndef BASE_H_
#define BASE_H_

using namespace std;

class Base {
public:
	Base(){
		src_extension[LANG_CPP]=".cpp";
		src_extension[LANG_C]=".c";
		src_extension[LANG_JAVA]=".java";

		exc_extension[LANG_CPP]=".out";
		exc_extension[LANG_C]=".out";
		exc_extension[LANG_JAVA]=".class";
	}
private:
protected:
	string toString(int n);
	string toString(char *str);
	string toString(const char *str);
	static const int LANG_CPP = 0;
	static const int LANG_C = 1;
	static const int LANG_JAVA =2;
	string src_extension[10];
	string exc_extension[10];
};

#endif /* BASE_H_ */
