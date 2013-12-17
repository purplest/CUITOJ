/*
 * Base.cpp
 *
 *  Created on: 2013-2-7
 *      Author: purple
 */
#include "Base.h"

using namespace std;

string Base::toString(int n){
	char str[25];
	sprintf(str,"%d",n);
	return string(str);
}

string Base::toString(char *str){
	return string(str);
}
string Base::toString(const char *str){
	return string(str);
}
