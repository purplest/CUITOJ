/*
 * judger.h
 *
 *  Created on: 2013-2-7
 *      Author: purple
 */

#ifndef JUDGER_H_
#define JUDGER_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "Log.h"
#include "Mysql.h"
#include "Program.h"
#include "Status.h"
#include "Base.h"

class Judger : public Base {
public:
	void server();
	Judger();
	~Judger();
private:
	static const int PORT;
	static const int MAXTHREAD;
	static const int MAXBUFFSIZE;

	int client_msg;
	int serversock;
	struct sockaddr_in remote_addr;
	void connect();// bind and listen the socket

	Program *program;
};


#endif /* JUDGER_H_ */
