/*
 * socket.cpp
 *
 *  Created on: 2013-2-15
 *      Author: purple
 */

#include "Judger.h"

void init(){
	Log::getInstance()->setFilename("Log.log");
}

int main(){
	init();
	Judger judger;
	judger.server();
	return 0;
}
